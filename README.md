Windows usage:
    Server launch:
.\tcpServer\out\build\x64-RelWithDebInfo-2022\RelWithDebInfo\tcpServer.exe
    Client launch:
.\tcpClient\out\build\x64-RelWithDebInfo-2022\RelWithDebInfo\tcpClient.exe <X.X.X.X> <FILEPATH>  #X.X.X.X - Server IP

    FILE PATH EXAMPLE
programBinDir/../../../../filesToSend/1test.txt
    OR
./tcpClient/filesToSend/1test.txt

Linux usage:
    Server launch:
sudo ./tcpServer/build/tcpServer
    Client launch:
./tcpClient/build/tcpClient X.X.X.X #X.X.X.X -IP