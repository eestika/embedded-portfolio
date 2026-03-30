#ifndef PROTOCOL_H
#define PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

void protocol_init(void);
void protocol_process_line(const char *line);

#ifdef __cplusplus
}
#endif

#endif /* PROTOCOL_H */
