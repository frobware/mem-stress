FROM gcc:latest as builder
WORKDIR /workdir
COPY . .
RUN make clean mem-stress
FROM scratch
COPY --from=builder /workdir/mem-stress /usr/local/bin
ENTRYPOINT ["/usr/local/bin/mem-stress", "10"]
