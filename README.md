# client_server

# Client-Server Communication Protocol

## Client Message Protocol

<br>Header size: 2 bytes  
Payload size: 8 bytes  
Total size: 10 bytes<br>

| Field      | Size    | Description                    |
|------------|---------|--------------------------------|
| version    | 1 byte  | Protocol version of the client |
| reserve    | 1 bytes | Reserve                        |
| value      | 8 bytes | value being sent               |

- **`version`**: Identifies the version of the communication protocol used by the client.  
- **`value`**: A `double` value.

---

## Server Message Protocol

<br>Header size: 12 bytes  
Payload max size: 1400 bytes  
Max total size: 1412 bytes<br><br>

| Field            | Size       | Description                                             |
|------------------|------------|---------------------------------------------------------|
| type             | 1 byte     | paylod type: `0` it's error string, `1` array of double |
| protocol version | 1 byte     | Protocol version of the server                          |
| total size       | 4 bytes    | Total size (in bytes) of all values being sent          |
| chunk number     | 2 bytes    | Sequence number of the message chunk                    |
| hash sum         | 4 bytes    | Paylod's hash sum                                       |
| payload          | 1400 bytes | Array of bytes                                          |

- **`type`**:  
  - `0`: Indicates an error message.  
  - `1`: Type of the value is array of double.  
- **`protocol version`**: Identifies the version of the communication protocol used by the server.  
- **`total size`**: The total size (in bytes) of the `double` values sent by the server.  
- **`chunk number`**: A number representing the sequence of the message.  
- **`payload`**:  
  - If `type` is `1`, contains an array of `double` values.  
  - If `type` is `0`, contains an error message and the protocol version of the server.

---

## Run an app
To run an app, it's necessary to add a path to a config.json file

Example:

`./server ../../config.json`

`./client ~/client/config.json`