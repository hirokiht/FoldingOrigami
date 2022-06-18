// on data will trigger SIGIO
class Serial
{
public:
    Serial(const char *tty, unsigned baudrate);
    char read();
private:
    int fd;
};
