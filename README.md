# client_server

# Client-Server Communication Protocol

## Client Message Protocol

|.........................Message.....................|  
|protocol version|version| value |  
|..........1 byte..........|1 byte |8 bytes|  
<br>Total message size: 10 bytes<br>

| Field             | Size     | Description                                                                 |
|------------------|----------|-----------------------------------------------------------------------------|
| protocol version | 1 byte   | Protocol version of the client                                              |
| version          | 1 byte   | Represents `QDataStream::version()`; both client and server must match it   |
| value            | 8 bytes  | A double value                                                              |

- **`protocol version`**: Identifies the version of the communication protocol used by the client.  
- **`version`**: Represents the `QDataStream::version()`. The client and server must use the same version to ensure that transferred data is deserialized correctly.  
- **`value`**: A `double` value.

---

## Server Message Protocol

|...................................................Message...............................................|  
|status|protocol version|total size|chunk number| payload |  
|1 byte|.........1 byte...........| 4 bytes  |........4 bytes........| ? bytes |  
<br>Total message size: 10 bytes<br>

| Field             | Size     | Description                                                                 |
|------------------|----------|-----------------------------------------------------------------------------|
| status           | 1 byte   | `0` for error, `1` for success                                               |
| protocol version | 1 byte   | Protocol version of the server                                              |
| total size       | 4 bytes  | Total size of all double values being sent                                  |
| chunk number     | 4 bytes  | Sequence number of the message chunk                                        |
| payload          | ? bytes  | Array of `double` values or an error message                                |

- **`status`**:  
  - `0`: Indicates an error message.  
  - `1`: Indicates successful data transmission.  
- **`protocol version`**: Identifies the version of the communication protocol used by the server.  
- **`total size`**: The total size (in bytes) of the `double` values sent by the server.  
- **`chunk number`**: A number representing the sequence of the message.  
- **`payload`**:  
  - If `status` is `1`, contains an array of `double` values.  
  - If `status` is `0`, contains an error message and the protocol version of the server.

---

Make sure both client and server use compatible protocol and `QDataStream` versions to avoid deserialization issues.
