#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

#define die(str, args...) do { \
    perror(str); \
    exit(EXIT_FAILURE); \
} while(0)

/*****************************************************************************/

static void emit(int fd, int type, int code, int val)
{
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = val;
    /* timestamp values below are ignored */
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    write(fd, &ie, sizeof(ie));
}

static int open_uinput(const char *name, int keycode)
{
    int fd;
    struct uinput_user_dev uidev;
    struct input_event ev;
    int i;

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        die("error: open");

    if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        die("error: ioctl");
    if(ioctl(fd, UI_SET_KEYBIT, keycode) < 0)
        die("error: ioctl");

    memset(&uidev, 0, sizeof(uidev));
    strncpy(uidev.name, name, UINPUT_MAX_NAME_SIZE);
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    if(write(fd, &uidev, sizeof(uidev)) < 0)
        die("error: write");

    if(ioctl(fd, UI_DEV_CREATE) < 0)
        die("error: ioctl");

    return fd;
}

static void close_uinput(int fd)
{
    if(ioctl(fd, UI_DEV_DESTROY) < 0)
        die("error: ioctl");

    close(fd);
}

static void usage(const char *program)
{
    fprintf(stderr, "%s: <keycode> <press-time-in-ms>\n", program);
}

int main(int argc, char **argv)
{
    int fd;
    int keycode = 0;
    int duration = 0;

    if (argc != 3) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    
    keycode = atoi(argv[1]);
    duration = atoi(argv[2]);
    if (duration < 100)
        duration = 100;
    
    fd = open_uinput("uroad-vkey", keycode);
    
    printf("key[%d] will be pressed for %dms.\n", keycode, duration);
    emit(fd, EV_KEY, keycode, 1);
    emit(fd, EV_SYN, SYN_REPORT, 0);
    
    usleep(duration * 1000);

    emit(fd, EV_KEY, keycode, 0);
    emit(fd, EV_SYN, SYN_REPORT, 0);
    
    close_uinput(fd);

    return 0;
}

