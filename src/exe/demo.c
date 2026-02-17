#include <stdio.h>

int main() {
    
    char cmd[128];
    fgets(cmd, sizeof(cmd), stdin);
    char rdrct_path[128];

    int quoted = 0;
    int dquoted = 0;
    int i_d = 0;
    int *i = &i_d;
    int path_len = 0;
    
    printf("%s\n", cmd);

    while (cmd[*i]!='\0' && !(!quoted && !dquoted && cmd[*i]==' ')){
        if (!quoted && !dquoted && cmd[*i]=='\"') {
            dquoted = 1;
            goto while_next1;
        }
        // (*i) ++;
        
        if (!quoted && dquoted && cmd[*i]=='\"') {
            dquoted = 0;
            goto while_next1;
        }

        if (!dquoted && !quoted && cmd[*i]=='\'') {
            quoted = 1;
            goto while_next1;
        }

        if (!dquoted && quoted && cmd[*i]=='\'') {
            quoted = 0;
            goto while_next1;
        }

        if (!quoted && cmd[*i]=='\\') {
            (*i) ++;
            rdrct_path[path_len++] = cmd[*i];
            goto while_next1;
        }

        rdrct_path[path_len++] = cmd[*i];

while_next1:
        (*i) ++;
//         printf("%d\n", *i);
    }
    
    printf("%d\n", *i);
    rdrct_path[path_len++] = '\0';
    for (int j=0; j<path_len; j++) {
        printf("%c\n", rdrct_path[j]);
    }
    printf("\n%s\n", rdrct_path);
    return 0;
}
