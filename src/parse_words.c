#include "../include/parse_words.h"
#include <string.h>
#include "../include/lexer.h"


static inline void track_char(Stream *s, const int32_t c) {
    if (c == '\n') {
        s->line++;
        s->column = 0;
    } else if (c != EOF) {
        s->column++;
    }
}


static inline int next_char(Stream *s) {
    const int32_t c = fgetc(s->fp);
    track_char(s, c);
    return c;
}


int parse_stream(Stream stream) {
    while (stream.peek != -1) {
        stream.peek = next_char(&stream);

        if (stream.peek == EOF) {
            break;
        }

        if (stream.peek == '/') {
            const int next = next_char(&stream);
            if (next == '/') {
                while (stream.peek != '\n' && stream.peek != -1) {
                    stream.peek = next_char(&stream);
                }
                continue;
            }
            if (next == '*') {
                while (true) {
                    stream.peek = next_char(&stream);
                    if (stream.peek == EOF) {
                        break;
                    }
                    if (stream.peek == '*') {
                        const int after = next_char(&stream);
                        if (after == '/') {
                            break;
                        }
                        ungetc(after, stream.fp);
                        stream.column--;
                    }
                }
                continue;
            }
            if (stream.pos == 0) {
                stream.token_line = stream.line;
                stream.token_column = stream.column;
            }
            stream.buf[stream.pos] = '\0';
            append_token(stream.buf, stream.token_line, stream.token_column);
            stream.pos = 0;
            continue;
        }

        if (stream.peek == '"') {
            if (stream.pos > 0) {
                stream.buf[stream.pos] = '\0';
                append_token(stream.buf, stream.token_line, stream.token_column);
                stream.pos = 0;
            }
            stream.token_line = stream.line;
            stream.token_column = stream.column;
            stream.buf[stream.pos++] = '"';
            while ((stream.peek = next_char(&stream)) != '"' && stream.peek != -1) {
                stream.buf[stream.pos++] = (char)stream.peek;
            }
            stream.buf[stream.pos++] = '"';
            stream.buf[stream.pos] = '\0';
            append_token(stream.buf, stream.token_line, stream.token_column);
            stream.pos = 0;
            continue;
        }

        if (stream.peek == ' ' || stream.peek == '\n') {
            if (stream.pos > 0) {
                stream.buf[stream.pos] = '\0';
                if (strcmp(stream.buf, "llvm") == 0) {
                    stream.pos = 0;
                    append_token("llvm", stream.token_line, stream.token_column);
                    int next;
                    while ((next = next_char(&stream)) != EOF) {
                        if (next == '{') {
                            break;
                        }
                        if (next == '/') {
                            const int after = next_char(&stream);
                            if (after == '/') {
                                while (next != '\n' && next != EOF) {
                                    next = next_char(&stream);
                                }
                                continue;
                            }
                            if (after == '*') {
                                while ((next = next_char(&stream)) != EOF) {
                                    if (next == '*') {
                                        const int closing = next_char(&stream);
                                        if (closing == '/') {
                                            break;
                                        }
                                        ungetc(closing, stream.fp);
                                        stream.column--;
                                    }
                                }
                                continue;
                            }
                            ungetc(after, stream.fp);
                            stream.column--;
                        }
                    }
                    if (next == '{') {
                        int depth = 1;
                        stream.pos = 0;
                        stream.token_line = stream.line;
                        stream.token_column = stream.column;
                        while (depth > 0) {
                            next = next_char(&stream);
                            if (next == EOF) {
                                return 1;
                            }
                            if (next == '/') {
                                const int after = next_char(&stream);
                                if (after == '/') {
                                    while ((next = next_char(&stream)) != EOF && next != '\n') {
                                    }
                                    continue;
                                }
                                if (after == '*') {
                                    while ((next = next_char(&stream)) != EOF) {
                                        if (next == '*') {
                                            const int closing = next_char(&stream);
                                            if (closing == EOF) {
                                                return 1;
                                            }
                                            if (closing == '/') {
                                                break;
                                            }
                                            ungetc(closing, stream.fp);
                                            stream.column--;
                                        }
                                    }
                                    continue;
                                }
                                ungetc(after, stream.fp);
                                stream.column--;
                                next = '/';
                            }
                            if (next == '"') {
                                stream.buf[stream.pos++] = (char)next;
                                while ((next = next_char(&stream)) != EOF) {
                                    stream.buf[stream.pos++] = (char)next;
                                    if (next == '\\') {
                                        next = next_char(&stream);
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
                        append_token_special(stream.buf, 0x060A, stream.token_line, stream.token_column);
                        stream.pos = 0;
                    }
                    continue;
                }

                if (strcmp(stream.buf, "clang") == 0) {
                    stream.pos = 0;
                    append_token("clang", stream.token_line, stream.token_column);
                    int next;
                    while ((next = next_char(&stream)) != EOF) {
                        if (next == '{') {
                            break;
                        }
                        if (next == '/') {
                            const int after = next_char(&stream);
                            if (after == '/') {
                                while (next != '\n' && next != EOF) {
                                    next = next_char(&stream);
                                }
                                continue;
                            }
                            if (after == '*') {
                                while ((next = next_char(&stream)) != EOF) {
                                    if (next == '*') {
                                        const int closing = next_char(&stream);
                                        if (closing == '/') {
                                            break;
                                        }
                                        ungetc(closing, stream.fp);
                                        stream.column--;
                                    }
                                }
                                continue;
                            }
                            ungetc(after, stream.fp);
                            stream.column--;
                        }
                    }
                    if (next == '{') {
                        int depth = 1;
                        stream.pos = 0;
                        stream.token_line = stream.line;
                        stream.token_column = stream.column;
                        while (depth > 0) {
                            next = next_char(&stream);
                            if (next == EOF) {
                                return 1;
                            }
                            if (next == '/') {
                                const int after = next_char(&stream);
                                if (after == '/') {
                                    while ((next = next_char(&stream)) != EOF && next != '\n') {
                                    }
                                    continue;
                                }
                                if (after == '*') {
                                    while ((next = next_char(&stream)) != EOF) {
                                        if (next == '*') {
                                            const int closing = next_char(&stream);
                                            if (closing == EOF) {
                                                return 1;
                                            }
                                            if (closing == '/') {
                                                break;
                                            }
                                            ungetc(closing, stream.fp);
                                            stream.column--;
                                        }
                                    }
                                    continue;
                                }
                                ungetc(after, stream.fp);
                                stream.column--;
                                next = '/';
                            }
                            if (next == '"') {
                                stream.buf[stream.pos++] = (char)next;
                                while ((next = next_char(&stream)) != EOF) {
                                    stream.buf[stream.pos++] = (char)next;
                                    if (next == '\\') {
                                        next = next_char(&stream);
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
                        append_token_special(stream.buf, 0x060B, stream.token_line, stream.token_column);
                        stream.pos = 0;
                    }
                    continue;
                }

                if (strcmp(stream.buf, "asm") == 0) {
                    stream.pos = 0;
                    append_token("asm", stream.token_line, stream.token_column);
                    int next;
                    while ((next = next_char(&stream)) != EOF) {
                        if (next == '{') {
                            break;
                        }
                        if (next == '/') {
                            const int after = next_char(&stream);
                            if (after == '/') {
                                while (next != '\n' && next != EOF) {
                                    next = next_char(&stream);
                                }
                                continue;
                            }
                            if (after == '*') {
                                while ((next = next_char(&stream)) != EOF) {
                                    if (next == '*') {
                                        const int closing = next_char(&stream);
                                        if (closing == '/') {
                                            break;
                                        }
                                        ungetc(closing, stream.fp);
                                        stream.column--;
                                    }
                                }
                                continue;
                            }
                            ungetc(after, stream.fp);
                            stream.column--;
                        }
                    }
                    if (next == '{') {
                        int depth = 1;
                        stream.pos = 0;
                        stream.token_line = stream.line;
                        stream.token_column = stream.column;
                        while (depth > 0) {
                            next = next_char(&stream);
                            if (next == EOF) {
                                return 1;
                            }
                            if (next == '/') {
                                const int after = next_char(&stream);
                                if (after == '/') {
                                    while ((next = next_char(&stream)) != EOF && next != '\n') {
                                    }
                                    continue;
                                }
                                if (after == '*') {
                                    stream.buf[stream.pos++] = '/';
                                    stream.buf[stream.pos++] = '*';
                                    while ((next = next_char(&stream)) != EOF) {
                                        stream.buf[stream.pos++] = (char)next;
                                        if (next == '*') {
                                            const int closing = next_char(&stream);
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
                                stream.column--;
                                next = '/';
                            }
                            if (next == '"') {
                                stream.buf[stream.pos++] = (char)next;
                                while ((next = next_char(&stream)) != EOF) {
                                    stream.buf[stream.pos++] = (char)next;
                                    if (next == '\\') {
                                        next = next_char(&stream);
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
                                while ((next = next_char(&stream)) != ' ' && next != '=' && next != EOF) {
                                    directive[di++] = (char)next;
                                }
                                directive[di] = '\0';
                                if (strcmp(directive, "arch") == 0) {
                                    while ((next = next_char(&stream)) == ' ' || next == '=') {
                                    }
                                    char arch[32];
                                    int ai = 0;
                                    while (next != ' ' && next != '{' && next != EOF) {
                                        arch[ai++] = (char)next;
                                        next = next_char(&stream);
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
                                    next = next_char(&stream);
                                }
                                int inner_depth = 1;
                                stream.pos = 0;
                                stream.token_line = stream.line;
                                stream.token_column = stream.column;
                                while (inner_depth > 0) {
                                    next = next_char(&stream);
                                    if (next == EOF) {
                                        return 1;
                                    }
                                    if (next == '/') {
                                        const int after = next_char(&stream);
                                        if (after == '/') {
                                            while ((next = next_char(&stream)) != EOF && next != '\n') {
                                            }
                                            continue;
                                        }
                                        if (after == '*') {
                                            while ((next = next_char(&stream)) != EOF) {
                                                if (next == '*') {
                                                    const int closing = next_char(&stream);
                                                    if (closing == EOF) {
                                                        return 1;
                                                    }
                                                    if (closing == '/') {
                                                        break;
                                                    }
                                                    ungetc(closing, stream.fp);
                                                    stream.column--;
                                                }
                                            }
                                            continue;
                                        }
                                        ungetc(after, stream.fp);
                                        stream.column--;
                                        next = '/';
                                    }
                                    if (next == '"') {
                                        stream.buf[stream.pos++] = (char)next;
                                        while ((next = next_char(&stream)) != EOF) {
                                            stream.buf[stream.pos++] = (char)next;
                                            if (next == '\\') {
                                                next = next_char(&stream);
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
                                append_token_special(stream.buf, arch_type, stream.token_line, stream.token_column);
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
                append_token(stream.buf, stream.token_line, stream.token_column);
                stream.pos = 0;
            }
            continue;
        }

        if (stream.peek == '(' || stream.peek == ')' || stream.peek == '{' ||
            stream.peek == '}' || stream.peek == '[' || stream.peek == ']' ||
            stream.peek == ';' || stream.peek == ':' || stream.peek == ',') {
            if (stream.pos > 0) {
                stream.buf[stream.pos] = '\0';
                append_token(stream.buf, stream.token_line, stream.token_column);
                stream.pos = 0;
            }
            stream.token_line = stream.line;
            stream.token_column = stream.column;
            stream.buf[0] = (char)stream.peek;
            stream.buf[1] = '\0';
            append_token(stream.buf, stream.token_line, stream.token_column);
            stream.pos = 0;
            continue;
        }

        if (stream.pos == 0) {
            stream.token_line = stream.line;
            stream.token_column = stream.column;
        }
        stream.buf[stream.pos] = (char)stream.peek;
        stream.pos++;
    }

    if (stream.pos > 0) {
        stream.buf[stream.pos] = '\0';
        append_token(stream.buf, stream.token_line, stream.token_column);
    }
    fclose(stream.fp);
    return 0;
}