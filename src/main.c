#define LIBSSH_STATIC 1
#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Result {
    union {
        char *err;
        char *result;
    };
    int is_err;
} Result;

/**
 * Only one of `err` or `result` should be NULL.
 */
Result new_result(char *err, char *result) {
    if (result != NULL) {
        return (Result){{result}, 0};
    }

    return (Result){{err}, 1};
}

Result ls_dir(ssh_session session, char *dir, char *flags);

int main(void) {
    int code;

    printf("SSH version: %s\n", ssh_version(0));

    char host[255] = "";
    unsigned int port;
    char username[255] = "";

    // Get input data
    printf("Example data:\n\tHost: \"test.rebex.net\"\n\tPort: 22\n\t"
           "User: \"demo\"\n(Press Enter to use the example data)\n\nInput your data:\n");
    printf("Host: ");
    char *host_ret = fgets(host, sizeof(host), stdin);
    if (*host_ret == '\n') {
        strcpy(host, "test.rebex.net");
    } else {
        host[strlen(host) - 1] = '\0';
    }

    char port_in[6] = "";
    printf("Port: ");
    char *port_ret = fgets(port_in, sizeof(port_in), stdin);
    if (*port_ret == '\n') {
        port = 22;
    } else {
        port_in[strlen(port_in) - 1] = '\0';
        port = atoi(port_in);
    }

    printf("User: ");
    char *user_ret = fgets(username, sizeof(username), stdin);
    if (*user_ret == '\n') {
        strcpy(username, "demo");
    } else {
        username[strlen(username) - 1] = '\0';
    }

    ssh_session session = ssh_new();
    if (session == NULL) {
        return 1;
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, &host);
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(session, SSH_OPTIONS_USER, &username);

    code = ssh_connect(session);
    if (code != SSH_OK) {
        fprintf(stderr, "Error connecting to %s: %s\n", host, ssh_get_error(session));
        goto error;
    }

    // Authenticate
    char *password = getpass("Password: ");
    code = ssh_userauth_password(session, username, password);
    if (code != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(session));
        goto error;
    }

    // Ask which directory to list
    char dir[255];
    printf("Which directory do you want to list?\n"
           "Just press enter to list the \"/\" and the \"pub/example\" directory.\n"
           "Directory: ");
    char *dir_ret = fgets(dir, sizeof(dir), stdin);
    if (*dir_ret != '\n') {
        dir[strlen(dir) - 1] = '\0';
        printf("\n\nContent and permissions of the \"%s\" directory:\n", dir);
        Result user_ls_la_res = ls_dir(session, dir, "-la");
        if (user_ls_la_res.is_err) {
            fprintf(stderr, "%s", user_ls_la_res.err);
            goto error;
        }
    } else {
        printf("\n\nContent and permissions of the \"/\" directory:\n");
        Result ls_la_res = ls_dir(session, "/", "-la");
        if (ls_la_res.is_err) {
            fprintf(stderr, "%s", ls_la_res.err);
            goto error;
        }

        printf("\n\nContent of the \"pub/example\" directory:\n");
        Result ls_res = ls_dir(session, "pub/example", NULL);
        if (ls_res.is_err) {
            fprintf(stderr, "%s", ls_res.err);
            goto error;
        }
    }

    printf("\n\nBye bye!\n(☞ಠ_ಠ)☞\n");
    ssh_disconnect(session);
    ssh_free(session);
    return 0;

error:
    fprintf(stderr, "Error!!!\n(╯°□°)╯︵ ┻━┻\n");
    ssh_disconnect(session);
    ssh_free(session);
    return 1;
}

Result ls_dir(ssh_session session, char *dir, char *flags) {
    Result result;
    int code;

    ssh_channel channel = ssh_channel_new(session);
    if (channel == NULL) {
        return new_result("Error creating channel.", NULL);
    }

    code = ssh_channel_open_session(channel);
    if (code != SSH_OK) {
        result = new_result("Error opening session in channel?!", NULL);
        goto error;
    }

    char lsla[8] = "ls ";
    char *command = malloc(strlen(lsla) + strlen(dir) + 1);
    strcpy(command, lsla);
    if (flags != NULL) {
        strcat(command, flags);
        strcat(command, " ");
    }
    strcat(command, dir);
    code = ssh_channel_request_exec(channel, command);
    free(command);
    if (code != SSH_OK) {
        result = new_result("Error executing command via ssh", NULL);
        goto error;
    }

    char buffer[512];
    int byte_count = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    while (byte_count > 0) {
        if (write(1, buffer, byte_count) != (unsigned int)byte_count) {
            result = new_result("Error writing command result", NULL);
            goto error;
        }
        byte_count = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    }

    if (byte_count < 0) {
        result = new_result("Error reading command result", NULL);
        goto error;
    }

    if (strlen(buffer) == 0) {
        result = new_result("No result from command", NULL);
        goto error;
    }

    result = new_result(NULL, buffer);
    ssh_channel_send_eof(channel);
    return result;

error:
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    return result;
}
