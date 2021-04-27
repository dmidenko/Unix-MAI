#!/bin/sh

INSTALL_PATH=/etc/tmwriter
EXEC_PATH="$INSTALL_PATH/writer.sh"

echo "creating install dir: $INSTALL_PATH"
mkdir -p "$INSTALL_PATH"
cp writer.sh "$EXEC_PATH"

chmod +x "$EXEC_PATH"


echo "creating system service with next config"
sed 's,PATH,'"$EXEC_PATH," ./service.template > /etc/systemd/system/tmwriter.service
cat /etc/systemd/system/tmwriter.service

echo "starting service"
systemctl start tmwriter.service


journalctl -u tmwriter.service -f
