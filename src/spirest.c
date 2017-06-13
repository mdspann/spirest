#include "mongoose.h"
#include "si7021.h"

static const char *s_http_port = "8000";
typedef enum content_encoding_e { ENCODING_TEXT, ENCODING_JSON } CONTENT_ENCODING;

/** @brief Determine what format the client wants the response.
 *
 * Currently supported options are TEXT and JSON.  Default to TEXT unless we
 * specifically recognize the request for JSON.
 *
 *  @param hm mongoose html message
 *  @return 0 for text, 1 for json
 */

static CONTENT_ENCODING requested_content_encoding(struct http_message *hm)
{
    struct mg_str *host_hdr;
    CONTENT_ENCODING content_encoding = ENCODING_TEXT;	// default to text

    host_hdr = mg_get_http_header(hm, "Accept");
    if (host_hdr) {
	if (!mg_vcmp(host_hdr, "application/json")) {
	    content_encoding = ENCODING_JSON;
	}
    }
    return content_encoding;
}

/** @brief Send a HTTP response with header and body.
 *
 * The caller is responsible for encoding the textual response
 *
 *  @param nc mongoose network connect
 *  @param content_encoding  Specify which content-encoding header to send.
 *  @param response HTTP response in proper encoding.
 */

static void send_result(struct mg_connection *nc,
			const CONTENT_ENCODING content_encoding,
			const char *response)
{
    int il = strlen(response);
    if (content_encoding) {
	mg_send_head(nc, 200, il, "Content-Encoding: application/json");
    } else {
	mg_send_head(nc, 200, il, "Content-Encoding: text/plain");
    }
    mg_send(nc, response, il);
}

/** @brief Mongoose event handler
 *
 * Mongoose has received an event.  
 *
 *  The only event we handle is an HTTP request so decode the URI and do
 *  something appropriate.
 *
 *  @param nc mongoose network connection
 *  @param ev Mongoose event code
 *  @return 0 for text, 1 for json
 */

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data)
{
    struct http_message *hm = (struct http_message *) ev_data;
    int content_encoding = ENCODING_TEXT;
    int status;
    char buff[1024];

    switch (ev) {
    case MG_EV_HTTP_REQUEST:

	if (!mg_vcmp(&hm->method, "GET")) {
	    content_encoding = requested_content_encoding(hm);

	    if (mg_vcmp(&hm->uri, "/sensor") == 0) {

		//  normally we would support multiple sensors with a variety
		//  of attributes, but for now, lets do the one for the PoC.

		// read the current temperature from the Si7021 temperature sensor

		float c;
		status = si7021_read_temperature(&c);
		if (status) {
		    mg_http_send_error(nc, 400, "Faild to read sensor");
		} else {

		    // We have the temperature, format it in either JSON or
		    // Text and send the answer.

		    if (content_encoding) {
			snprintf(buff, sizeof(buff),
				 "[ {\"Temperature:\",\"%f\"} ]\n", c);
			send_result(nc, content_encoding, buff);
		    } else {
			snprintf(buff, sizeof(buff), "Temperature:%f\n",
				 c);
			send_result(nc, content_encoding, buff);
		    }
		}
	    } else {
		// unsupported URI.
		mg_http_send_error(nc, 404, "Unknown request");
	    }
	} else {
	    // Unsupport HTTP method.
	    mg_http_send_error(nc, 405, "Unknown method");
	}
	break;
    default:
	break;
    }
}

int main()
{
    struct mg_mgr mgr;
    struct mg_connection *nc;

    mg_mgr_init(&mgr, NULL);

    nc = mg_bind(&mgr, s_http_port, ev_handler);
    if (nc == NULL) {
	fprintf(stderr, "Error starting server on port %s\n", s_http_port);
	exit(1);
    }

    mg_set_protocol_http_websocket(nc);

    printf("Starting RESTful server on port %s\n", s_http_port);
    for (;;) {
	mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);

    return 0;
}
