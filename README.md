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
sntp-server -i localhost -v
````

Enhanced privileges (e.g. sudo) might be required to successfully bind to SNTP port (123 UDP).

| Option          | Example                 | Description |
| --------------- | ----------------------- | ----------- |
| -i, --interface | -i localhost            | specify local interface to bind (default: any) |
| -t, --time      | -t 2022-03-10T18:43:00Z | specify time to reply (default: system time) |
| -s, --stratum   | -s 1                    | specity stratum to reply (range: 0..15) |
| -v, --verbode   | -v                      | print SNTP replies |
| -h, --help      | -h                      | print usage and exit |

## Run example client

````
sntp-client localhost
````

The example clients requests the time form the server provided as argument. It will print the results and does not perform any changes on the local system.

| Option        | Example | Description |
| ------------- | ------- | ----------- |
| -t, --timeout | -t 1000 | specify receive timeout in milli seconds (default: 5000) |

## Intended purpose

The library is intended for testing only, e.g. provide a local SNTP server an play around with some options, e.g. stratum.  
It is not intended for productive usage.

## Known limitations

- IPv4 only  
  The current implementations does not support IPv6.
- no authentication  
  SNTP authetication is not supported. The optional authentication field is ignored in requests and omitted in replies.
- timestamp fractions not displayed  
  Timestamp fractions are not displayed in packet dumps, but they are provided in the packet struct.

## References

- [RFC 4330: SNTPv4](https://datatracker.ietf.org/doc/html/rfc4330)  
  Note that SNTPv4 is also part of [RFC 5905: NTPv4](https://datatracker.ietf.org/doc/html/rfc5905), but I refer to the older spec since it is more focused on the packet format.