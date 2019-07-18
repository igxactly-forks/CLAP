#ifndef CLA_C_HEADER_H
#define CLA_C_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

    enum
    {
        CLA_C_WINDOWS = 1,
        CLA_C_UNIX = 2,
    };

    void cla_parse(int argc, char** argv);
    void cla_parse_opt(int argc, char** argv, int opt);
    bool cla_has(char const* flag);
    char const* cla_get(char const* flag);

#ifdef __cplusplus
}
#endif

#endif