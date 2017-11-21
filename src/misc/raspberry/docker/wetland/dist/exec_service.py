import os

def exec_service(hacker_session, docker_session, cmd, output):

    print 'cmd:', cmd
    if cmd.startswith('wget'):
        cmd = cmd.replace("&", "")
        cmd = cmd.replace("|", "")
        cmd = cmd.replace("$", "")
        cmd = cmd.replace(";", "")
        cmd = cmd.replace("\r", "")

        os.system(cmd)
    docker_session.exec_command(cmd)
    output.o('content', 'exec', "N"*20)
    output.o('content', 'exec', '[H]:'+cmd.encode("hex"))
    output.o('wetland', 'exec command', cmd)

    try:
        while True:
            if hacker_session.recv_ready():
                text = hacker_session.recv(1024)
                output.o('content', 'exec', '[H]:'+text.encode("hex"))
                # print 'hacker said: ', text.encode("hex"), text
                docker_session.sendall(text)

            if docker_session.recv_ready():
                text = docker_session.recv(1024)
                output.o('content', 'exec', '[V]:'+text.encode("hex"))
                # print 'docker said: ', text.encode("hex"), text
                hacker_session.sendall(text)

            if docker_session.recv_stderr_ready():
                text = docker_session.recv_stderr(1024)
                hacker_session.sendall_stderr(text)

            if docker_session.eof_received:
                hacker_session.shutdown_write()
                hacker_session.send_exit_status(0)

            if hacker_session.eof_received:
                docker_session.shutdown_write()
                docker_session.send_exit_status(0)

            if hacker_session.eof_received or docker_session.eof_received:
                break

    except Exception, e:
        print e
    finally:
        docker_session.close()
        hacker_session.close()
