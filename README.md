# Socket Programming 
The client will create connection and send requests to download file from server. Progress will also be printed.

# Instructions Manual

1. For `server.c`:

   ```bash
   cd server
   gcc -o server server.c
   ```

2. For `client.c`:

   ```bash
   cd client
   gcc -o client client.c
   ```

3. Open the directories in different terminals and run the binaries in the respective directories by:

   ```bash
   ./server
   ```

   ```bash
   ./client
   ```

   

## Command Usage

These commands need to be run in client. If any command other than these are given then a messege will be printed.

### get
```bash
   Usage: get <filename-1> ... <filename-N>
   ```
- It will give error if no file is given.
- It can also handle multiple file requests.
### exit
   ` 
   Usage: exit`
* Connection between the client and server is lost and both the codes exit.