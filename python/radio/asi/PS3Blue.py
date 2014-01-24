import subprocess


def pair(config):
    cmdLine = [config.sixpair]
    code = subprocess.call(cmdLine)
    if code == 0:
        print("sixpair succeded")
    else:
        print("sixpair filed: exit code {0}".format(code))


def detach(mac):
    cmd = "sudo hcitool dc {0}".format(mac)
    print(cmd)
    raise Exception("detached")
