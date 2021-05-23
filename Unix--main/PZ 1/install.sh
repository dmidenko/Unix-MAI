#!/bin/sh

INSTALL_PATH=/etc/tmwriter
SCRIPT_PATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

EXEC_CMD="$(which python) $INSTALL_PATH/dbusmonitor.py"
#copy exec script into /etc/tmwriter
#making directory bsc it not exists

echo "creating install dir: $INSTALL_PATH"
mkdir -p "$INSTALL_PATH"
cp "$SCRIPT_PATH/dbusmonitor.py" "$INSTALL_PATH/"

# copyng service file to systemd folder, and change template with real path
# we replace path with exec_path wariable, and then writting it to systemd folder
# with > we write stdout to fetched file
# read docs about operator > in shell and about sed (its not difficult)

echo "creating system service with next config"
sed 's,PATH,'"$EXEC_CMD," ./service.template > /etc/systemd/system/tmwriter.service
cat /etc/systemd/system/tmwriter.service

# starting service with systemctl
echo "starting service"
systemctl start tmwriter.service

# all stdout and stderr from system service can be find ini journalctl (its logger)
# now u can see logs with the following command (if u wanna to end this script and write it urself, just comment next line)
journalctl -u tmwriter.service -f
