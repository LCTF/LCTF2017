From rastasheep/ubuntu-sshd
COPY dist /root/dist

RUN useradd -s /bin/bash -m -u 555 pi && \
    echo "pi:shumeipai" | chpasswd && \
    echo "root:8901234567qwertyuiop" | chpasswd && \
    mv /root/dist/flag.txt /home/pi && \
    chmod 644 /home/pi/flag.txt

CMD ["/usr/sbin/sshd", "-D"]
