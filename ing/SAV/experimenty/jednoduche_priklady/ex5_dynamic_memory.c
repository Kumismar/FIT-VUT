#include <stdlib.h>

int nondet_int();

typedef struct
{
    int id;
    int status;
} Session;

void do_something(Session* s)
{
    s->status = 1;
}

int handle_event(Session* s, int event)
{
    if (event == 404) {
        free(s);
        // forgot to return err value
    }
    return 0;
}

int main()
{
    Session* s = (Session*)malloc(sizeof(Session));
    if (!s)
        return 0;

    s->id = 1;
    s->status = 0;

    int event = nondet_int();
    if (handle_event(s, event) != 0) {
        exit(EXIT_FAILURE);
    }

    do_something(s);
    return 0;
}