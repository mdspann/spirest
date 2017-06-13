# spirest

Use the following as a problem statement:

I want to know the temperature of the computers in my garage.  Assume there is
power and network connection and assume a well equiped parts bin. Develop a
hardware and software solution to integrate the temperature in the garage on a
web page.

I built a simple REST server to sample devices on the SPI bus using mongoose as
the embedded web server.  The sample hardware is a pine64 ARM development board
but the code should work on any Raspberry PI or clone.

The only sensor currently supported is the
http://wiki.pine64.org/index.php/POT#I2C_Device:_Humidity_and_Temperature_Sensor_.28PMSDP01.29
although any product with a Silicon Labs Si7021 I2C Humidity and Temperature
Sensor should work.

Requires mongoose ( https://docs.cesanta.com/mongoose/master/ ) which has its own license.

Must run from an account with read/write access to SPI bus.

Example results follows:

# curl -v -X GET -H "Accept: application/json" pine64-2:8000/sensor
* Hostname was NOT found in DNS cache
*   Trying 192.168.7.98...
* Connected to pine64-2 (192.168.7.98) port 8000 (#0)
> GET /sensor HTTP/1.1
> User-Agent: curl/7.35.0
> Host: pine64-2:8000
> Accept: application/json
>
< HTTP/1.1 200 OK
* Server Mongoose/6.7 is not blacklisted
< Server: Mongoose/6.7
< Content-Encoding: application/json
< Content-Length: 33
<
[ {"Temperature:","24.410547"} ]

