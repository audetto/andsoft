import subprocess


def pair(config):
    cmdLine = [config.sixpair]
    code = subprocess.call(cmdLine)
    if code == 0:
        print("sixpair succeded")
    else:
        print("sixpair filed: exit code {0}".format(code))


def detach(mac):
    cmdLine = ["sudo", "hcitool", "dc", mac]
    code = subprocess.call(cmdLine)
    if code == 0:
        print("successfully detached")
    else:
        print("detach filed: exit code {0}".format(code))
