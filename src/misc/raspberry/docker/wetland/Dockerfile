FROM ohmyadd/wetland
COPY dist /root/dist

RUN adduser -D -u 500  wetland && \
    echo "wetland:wetland" | chpasswd && \
    cp -R /root/wetland /home/wetland/ && \
    cp /root/dist/exec_service.py /home/wetland/wetland/wetland/services/exec_service.py && \
    cp /root/dist/shell_service.py /home/wetland/wetland/wetland/services/shell_service.py && \
    chown -R wetland:wetland /home/wetland && \
    chown -R wetland /home/wetland/wetland/log && \
    chown  wetland /home/wetland/wetland/wetland.cfg && \
    sed -ri 's/wetland_port = 22/wetland_port = 2222/' /home/wetland/wetland/wetland.cfg && \
    sed -ri "s/root = '\/root\/wetland\/'/root = '\/home\/wetland\/wetland\/'/" /usr/local/bin/entrypoint.py && \
    
    cp /root/dist/flag.txt /home/wetland/flag.txt && \
    rm -rf /root/dist && \
    chmod 600 -R /root

VOLUME /home/wetland/wetland/log
USER wetland
WORKDIR /home/wetland
ENTRYPOINT ["entrypoint.py"]
