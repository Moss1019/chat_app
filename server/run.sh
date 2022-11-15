
FOLDER_EXISTS=false
[ -d "./build" ] && FOLDER_EXISTS=true

if $FOLDER_EXISTS
then
    echo "Folder already created"
    cd build
    make && ./chat_server
    cd ..
else
    echo "Folder not created"
    mkdir build
    cd build
    cmake ..
    make && ./chat_server
    cd ..
fi
