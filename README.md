# pseudo-sntp
Pseudo SNTP server for testing purposes.

## Build and install

````
mkdir build
cd build
make
sudo make install
````

## Run example server

````
sntp-server -i localhost
````

The example server will bind to the IP interface specified by `-i` option on SNTP Port (123 UDP). It provides the local time with precision of one second.

Enhanced privileges (e.g. sudo) might be required to successfully bind.

## Run example client

````
sntp-client localhost
````

The example clients requests the time form the server provided as argument. It will print the results and does not perform any changes on the local system.

## Intended purpose

The library is intended for testing only, e.g. provide a local SNTP server an play around with some options, e.g. stratum.  
It is not intended for productive usage.

## References

- [RFC 4330: SNTPv4](https://datatracker.ietf.org/doc/html/rfc4330)  
  Note that SNTPv4 is also part of [RFC 5905: NTPv4](https://datatracker.ietf.org/doc/html/rfc5905), but I refer to the older spec since it is more focused on the packet format.