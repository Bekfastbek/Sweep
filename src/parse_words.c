#include "../include/parse_words.h"
#include <string.h>
#include "../include/lexer.h"

int parse_stream(Stream stream) {
    while (stream.peek != -1) {
        stream.peek = fgetc(stream.fp);

        if (stream.peek == EOF) {
            break;
        }

        if (stream.peek == '/') {
            const int next = fgetc(stream.fp);
            if (next == '/') {
                while (stream.peek != '\n' && stream.peek != -1) {
                    stream.peek = fgetc(stream.fp);
                }
                continue;
            }
            if (next == '*') {
                while (true) {
                    stream.peek = fgetc(stream.fp);
                    if (stream.peek == EOF) {
                        break;
                    }
                    if (stream.peek == '*') {
                        const int after = fgetc(stream.fp);
                        if (after == '/') {
                            break;
                        }
                        ungetc(after, stream.fp);
                    }
                }
                continue;
            }
            stream.buf[stream.pos] = '\0';
            append_token(stream.buf);
            stream.pos = 0;
            continue;
        }

        if (stream.peek == '"') {
            if (stream.pos > 0) {
                stream.buf[stream.pos] = '\0';
                append_token(stream.buf);
                stream.pos = 0;
            }
            stream.buf[stream.pos++] = '"';
            while ((stream.peek = fgetc(stream.fp)) != '"' && stream.peek != -1) {
                stream.buf[stream.pos++] = (char)stream.peek;
            }
            stream.buf[stream.pos++] = '"';
            stream.buf[stream.pos] = '\0';
            append_token(stream.buf);
            stream.pos = 0;
            continue;
        }

        if (stream.peek == ' ' || stream.peek == '\n') {
            if (stream.pos > 0) {
                stream.buf[stream.pos] = '\0';
                if (strcmp(stream.buf, "llvm") == 0) {
                    stream.pos = 0;
                    append_token("llvm");
                    int next;
                    while ((next = fgetc(stream.fp)) != EOF) {
                        if (next == '{') {
                            break;
                        }
                        if (next == '/') {
                            const int after = fgetc(stream.fp);
                            if (after == '/') {
                                while (next != '\n' && next != EOF) {
                                    next = fgetc(stream.fp);
                                }
                                continue;
                            }
                            if (after == '*') {
                                while ((next = fgetc(stream.fp)) != EOF) {
                                    if (next == '*') {
                                        const int closing = fgetc(stream.fp);
                                        if (closing == '/') {
                                            break;
                                        }
                                        ungetc(closing, stream.fp);
                                    }
                                }
                                continue;
                            }
                            ungetc(after, stream.fp);
                        }
                    }
                    if (next == '{') {
                        int depth = 1;
                        stream.pos = 0;
                        while (depth > 0) {
                            next = fgetc(stream.fp);
                            if (next == EOF) {
                                return 1;
                            }
                            if (next == '/') {
                                const int after = fgetc(stream.fp);
                                if (after == '/') {
                                    while ((next = fgetc(stream.fp)) != EOF && next != '\n') {
                                    }
                                    continue;
                                }
                                if (after == '*') {
                                    while ((next = fgetc(stream.fp)) != EOF) {
                                        if (next == '*') {
                                            const int closing = fgetc(stream.fp);
                                            if (closing == EOF) {
                                                return 1;
                                            }
                                            if (closing == '/') {
                                                break;
                                            }
                                        }
                                    }
                                    continue;
                                }
                                ungetc(after, stream.fp);
                                next = '/';
                            }
                            if (next == '"') {
                                stream.buf[stream.pos++] = (char)next;
                                while ((next = fgetc(stream.fp)) != EOF) {
                                    stream.buf[stream.pos++] = (char)next;
                                    if (next == '\\') {
                                        next = fgetc(stream.fp);
                                        if (next == EOF) {
                                            return 1;
                                        }
                                        stream.buf[stream.pos++] = (char)next;
                                    } else if (next == '"') {
                                        break;
                                    }
                                }
                                continue;
                            }
                            if (next == '{') {
                                depth++;
                            }
                            if (next == '}') {
                                depth--;
                                if (depth == 0) {
                                    break;
                                }
                            }
                            stream.buf[stream.pos++] = (char)next;
                        }
                        stream.buf[stream.pos] = '\0';
                        append_token_special(stream.buf, 0x060A);
                        stream.pos = 0;
                    }
                    continue;
                }

                if (strcmp(stream.buf, "clang") == 0) {
                    stream.pos = 0;
                    append_token("clang");
                    int next;
                    while ((next = fgetc(stream.fp)) != EOF) {
                        if (next == '{') {
                            break;
                        }
                        if (next == '/') {
                            const int after = fgetc(stream.fp);
                            if (after == '/') {
                                while (next != '\n' && next != EOF) {
                                    next = fgetc(stream.fp);
                                }
                                continue;
                            }
                            if (after == '*') {
                                while ((next = fgetc(stream.fp)) != EOF) {
                                    if (next == '*') {
                                        const int closing = fgetc(stream.fp);
                                        if (closing == '/') {
                                            break;
                                        }
                                        ungetc(closing, stream.fp);
                                    }
                                }
                                continue;
                            }
                            ungetc(after, stream.fp);
                        }
                    }
                    if (next == '{') {
                        int depth = 1;
                        stream.pos = 0;
                        while (depth > 0) {
                            next = fgetc(stream.fp);
                            if (next == EOF) {
                                return 1;
                            }
                            if (next == '/') {
                                const int after = fgetc(stream.fp);
                                if (after == '/') {
                                    while ((next = fgetc(stream.fp)) != EOF && next != '\n') {
                                    }
                                    continue;
                                }
                                if (after == '*') {
                                    while ((next = fgetc(stream.fp)) != EOF) {
                                        if (next == '*') {
                                            const int closing = fgetc(stream.fp);
                                            if (closing == EOF) {
                                                return 1;
                                            }
                                            if (closing == '/') {
                                                break;
                                            }
                                        }
                                    }
                                    continue;
                                }
                                ungetc(after, stream.fp);
                                next = '/';
                            }
                            if (next == '"') {
                                stream.buf[stream.pos++] = (char)next;
                                while ((next = fgetc(stream.fp)) != EOF) {
                                    stream.buf[stream.pos++] = (char)next;
                                    if (next == '\\') {
                                        next = fgetc(stream.fp);
                                        if (next == EOF) {
                                            return 1;
                                        }
                                        stream.buf[stream.pos++] = (char)next;
                                    } else if (next == '"') {
                                        break;
                                    }
                                }
                                continue;
                            }
                            if (next == '{') {
                                depth++;
                            }
                            if (next == '}') {
                                depth--;
                                if (depth == 0) {
                                    break;
                                }
                            }
                            stream.buf[stream.pos++] = (char)next;
                        }
                        stream.buf[stream.pos] = '\0';
                        append_token_special(stream.buf, 0x060B);
                        stream.pos = 0;
                    }
                    continue;
                }

                if (strcmp(stream.buf, "asm") == 0) {
                    stream.pos = 0;
                    append_token("asm");
                    int next;
                    while ((next = fgetc(stream.fp)) != EOF) {
                        if (next == '{') {
                            break;
                        }
                        if (next == '/') {
                            const int after = fgetc(stream.fp);
                            if (after == '/') {
                                while (next != '\n' && next != EOF) {
                                    next = fgetc(stream.fp);
                                }
                                continue;
                            }
                            if (after == '*') {
                                while ((next = fgetc(stream.fp)) != EOF) {
                                    if (next == '*') {
                                        const int closing = fgetc(stream.fp);
                                        if (closing == '/') {
                                            break;
                                        }
                                        ungetc(closing, stream.fp);
                                    }
                                }
                                continue;
                            }
                            ungetc(after, stream.fp);
                        }
                    }
                    if (next == '{') {
                        int depth = 1;
                        stream.pos = 0;
                        while (depth > 0) {
                            next = fgetc(stream.fp);
                            if (next == EOF) {
                                return 1;
                            }
                            if (next == '/') {
                                const int after = fgetc(stream.fp);
                                if (after == '/') {
                                    while ((next = fgetc(stream.fp)) != EOF && next != '\n') {
                                    }
                                    continue;
                                }
                                if (after == '*') {
                                    stream.buf[stream.pos++] = '/';
                                    stream.buf[stream.pos++] = '*';
                                    while ((next = fgetc(stream.fp)) != EOF) {
                                        stream.buf[stream.pos++] = (char)next;
                                        if (next == '*') {
                                            const int closing = fgetc(stream.fp);
                                            if (closing == EOF) {
                                                return 1;
                                            }
                                            stream.buf[stream.pos++] = (char)closing;
                                            if (closing == '/') {
                                                break;
                                            }
                                        }
                                    }
                                    continue;
                                }
                                ungetc(after, stream.fp);
                                next = '/';
                            }
                            if (next == '"') {
                                stream.buf[stream.pos++] = (char)next;
                                while ((next = fgetc(stream.fp)) != EOF) {
                                    stream.buf[stream.pos++] = (char)next;
                                    if (next == '\\') {
                                        next = fgetc(stream.fp);
                                        if (next == EOF) {
                                            return 1;
                                        }
                                        stream.buf[stream.pos++] = (char)next;
                                    } else if (next == '"') {
                                        break;
                                    }
                                }
                                continue;
                            }
                            if (next == '@') {
                                uint16_t arch_type = 0;
                                char directive[32];
                                int di = 0;
                                while ((next = fgetc(stream.fp)) != ' ' && next != '=' && next != EOF) {
                                    directive[di++] = (char)next;
                                }
                                directive[di] = '\0';
                                if (strcmp(directive, "arch") == 0) {
                                    while ((next = fgetc(stream.fp)) == ' ' || next == '=') {
                                    }
                                    char arch[32];
                                    int ai = 0;
                                    while (next != ' ' && next != '{' && next != EOF) {
                                        arch[ai++] = (char)next;
                                        next = fgetc(stream.fp);
                                    }
                                    arch[ai] = '\0';
                                    if (strcmp(arch, "x86_64") == 0) {
                                        arch_type = 0x060C;
                                    } else if (strcmp(arch, "arm64") == 0) {
                                        arch_type = 0x060D;
                                    } else if (strcmp(arch, "x86") == 0) {
                                        arch_type = 0x060E;
                                    } else if (strcmp(arch, "arm32") == 0) {
                                        arch_type = 0x060F;
                                    }
                                } else {
                                    return 1;
                                }
                                while (next != '{' && next != EOF) {
                                    next = fgetc(stream.fp);
                                }
                                int inner_depth = 1;
                                stream.pos = 0;
                                while (inner_depth > 0) {
                                    next = fgetc(stream.fp);
                                    if (next == EOF) {
                                        return 1;
                                    }
                                    if (next == '/') {
                                        const int after = fgetc(stream.fp);
                                        if (after == '/') {
                                            while ((next = fgetc(stream.fp)) != EOF && next != '\n') {
                                            }
                                            continue;
                                        }
                                        if (after == '*') {
                                            while ((next = fgetc(stream.fp)) != EOF) {
                                                if (next == '*') {
                                                    const int closing = fgetc(stream.fp);
                                                    if (closing == EOF) {
                                                        return 1;
                                                    }
                                                    if (closing == '/') {
                                                        break;
                                                    }
                                                }
                                            }
                                            continue;
                                        }
                                        ungetc(after, stream.fp);
                                        next = '/';
                                    }
                                    if (next == '"') {
                                        stream.buf[stream.pos++] = (char)next;
                                        while ((next = fgetc(stream.fp)) != EOF) {
                                            stream.buf[stream.pos++] = (char)next;
                                            if (next == '\\') {
                                                next = fgetc(stream.fp);
                                                if (next == EOF) {
                                                    return 1;
                                                }
                                                stream.buf[stream.pos++] = (char)next;
                                            } else if (next == '"') {
                                                break;
                                            }
                                        }
                                        continue;
                                    }
                                    if (next == '{') {
                                        inner_depth++;
                                    }
                                    if (next == '}') {
                                        inner_depth--;
                                        if (inner_depth == 0) {
                                            break;
                                        }
                                    }
                                    stream.buf[stream.pos++] = (char)next;
                                }
                                stream.buf[stream.pos] = '\0';
                                append_token_special(stream.buf, arch_type);
                                stream.pos = 0;
                                continue;
                            }
                            if (next == '{') {
                                depth++;
                            }
                            if (next == '}') {
                                depth--;
                                if (depth == 0) {
                                    break;
                                }
                            }
                        }
                        continue;
                    }
                }
                append_token(stream.buf);
                stream.pos = 0;
            }
            continue;
        }

        if (stream.peek == '(' || stream.peek == ')' || stream.peek == '{' ||
            stream.peek == '}' || stream.peek == '[' || stream.peek == ']' ||
            stream.peek == ';' || stream.peek == ':' || stream.peek == ',') {
            if (stream.pos > 0) {
                stream.buf[stream.pos] = '\0';
                append_token(stream.buf);
                stream.pos = 0;
            }
            stream.buf[0] = (char)stream.peek;
            stream.buf[1] = '\0';
            append_token(stream.buf);
            stream.pos = 0;
            continue;
        }

        stream.buf[stream.pos] = (char)stream.peek;
        stream.pos++;
    }

    if (stream.pos > 0) {
        stream.buf[stream.pos] = '\0';
        append_token(stream.buf);
    }
    fclose(stream.fp);
    return 0;
}