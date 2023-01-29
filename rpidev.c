#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <newt.h>

#define TOOL_VERSION "0.0.1"

// Menu function
void mainMenu();    // Main menu
void networkMenu(); // network menu
void proxyTools();  // Proxy menu 

// Tool function
const char *readSysProxyInfo();
const char *generateEnv(char *result, const char *ip, const char *port, const char *user, const char *pass, const int way);
void setSysProxy(const char *ip, const char *port, const char *user, const char *pass, const int way);
void unsetSysProxy();

void inBuilding() { newtWinMessage("!ERROR!", "OK", "This function still in building!"); }

void main()
{   
    // char *a = readSysProxyInfo();
    // printf("Query:: %s", a);
    // system("/bin/bash && source ~/.bashrc");
    mainMenu();    
    // proxyTools();
}

void mainMenu() {
    char base[100] = "Raspberry Pi Development Toolbox - ";
    char *version = TOOL_VERSION;
    const char *mainTitle = strcat(base, version);
    newtComponent ls, fm;
    int p = 1, q = 2, r = 3, s = 4, t = 5, *u;
    newtInit();
    do
    {
        newtCls();
        newtRefresh();
        newtDrawRootText(0, 0, mainTitle);
        newtCenteredWindow(64, 10, "Main Menu");
        ls = newtListbox(18, 3, 5, NEWT_FLAG_RETURNEXIT);
        newtListboxAppendEntry(ls, "1.System Configuration", &p);
        newtListboxAppendEntry(ls, "2.Network Congiguration", &q);
        newtListboxAppendEntry(ls, "3.Environment Configuration", &r);
        newtListboxAppendEntry(ls, "4.Other Tools", &s);
        newtListboxAppendEntry(ls, "5.Quit", &t);
        newtPushHelpLine(" Move using the arrow keys and press ENTER to select");
        fm = newtForm(NULL, NULL, 0);
        newtFormAddComponents(fm, ls, NULL);
        newtRunForm(fm);
        u = newtListboxGetCurrent(ls);
        newtPopWindow();
        newtFormDestroy(fm);
        switch (*u)
        {
        case 1:
            // proxyToolsSys();
            inBuilding();
            break;
        case 2:
            networkMenu();
            break;
        case 3:
            // proxyToolsGit();
            inBuilding();
            break;
        case 4:
            // test();
            inBuilding();
            break;
        case 5:
            newtFinished();
            // system("clear");
            system("/bin/bash && source ~/.bashrc && clear");
            
        }
    } while (1);
}

void networkMenu() {
    char base[100] = "Raspberry Pi Development Toolbox - ";
    char *version = TOOL_VERSION;
    const char *mainTitle = strcat(base, version);
    newtComponent ls, fm;
    int p = 1, q = 2, r = 3, s = 4, t = 5, *u;
    newtInit();
    do
    {
        newtCls();
        newtRefresh();
        newtDrawRootText(0, 0, mainTitle);
        newtCenteredWindow(64, 10, "Network Toolbox");
        ls = newtListbox(18, 3, 5, NEWT_FLAG_RETURNEXIT);
        newtListboxAppendEntry(ls, "1.System Proxy Settings", &p);
        newtListboxAppendEntry(ls, "2.Git Proxy Settings", &q);
        newtListboxAppendEntry(ls, "3....", &r);
        newtListboxAppendEntry(ls, "4....", &s);
        newtListboxAppendEntry(ls, "5.Quit", &t);
        newtPushHelpLine("Move using the arrow keys and press ENTER to select");
        fm = newtForm(NULL, NULL, 0);
        newtFormAddComponents(fm, ls, NULL);
        newtRunForm(fm);
        u = newtListboxGetCurrent(ls);
        newtPopWindow();
        newtFormDestroy(fm);
        switch (*u)
        {
        case 1:
            proxyToolsSys();
            break;
        case 2:
            proxyToolsGit();
            break;
        case 3:
            break;
        case 4:
            // test();
            break;
        case 5:
            newtPopWindow();
            return;
        }
    } while (1);
}

const char *readSysProxyInfo() {
    int sysEnv = 0xFF;
    char *sys_info;
    const char *env_var_name = "http_proxy";
    sysEnv = getenv(env_var_name);
    if (sysEnv == 0) {
        return "null";
    } else {
        return getenv(env_var_name);
    }
}

const char *generateEnv(char *result, const char *ip, const char *port, const char *user, const char *pass, const int way) {
    if (way == 1) 
    {
        strcat(result, "http://");
    }
    else if (way == 2) 
    {
        strcat(result, "socks4://");
    }
    else if (way == 3) 
    {
        strcat(result, "socks5://");
    }
    if(strlen(user) > 0 && strlen(pass) > 0) 
    {
        strcat(result, user);
        strcat(result, ":");
        strcat(result, pass);
        strcat(result, "@");
    }
    if (port != NULL) 
    {
        strcat(result, ip);
        strcat(result, ":");
        strcat(result, port);
    }
    return result;
}

void unsetSysProxy() {
    // unsetenv("http_proxy");
    // unsetenv("https_proxy");
    //  awk '!(/^export http_proxy/) { print $0 }' .sim > .sim
    system("awk 'BEGIN{printf \"\" > \"temp\"} {if(!(/^export http_proxy/)) print $0 >> \"temp\";}' ~/.bashrc; mv temp ~/.bashrc");
    system("awk 'BEGIN{printf \"\" > \"temp\"} {if(!(/^export https_proxy/)) print $0 >> \"temp\";}' ~/.bashrc; mv temp ~/.bashrc");
    system("awk 'BEGIN{printf \"\" > \"temp\"} {if(!(/^export ftp_proxy/)) print $0 >> \"temp\";}' ~/.bashrc; mv temp ~/.bashrc");
    newtWinMessage("Info", "OK", "System proxy has been unset.");
}

void setSysProxy(const char *ip, const char *port, const char *user, const char *pass, const int way) {
    int len = strlen(ip) + strlen(port) + strlen(user) + strlen(pass) + 32;
    char *env_res = (char *)malloc(len * sizeof(char));
    memset(env_res, NULL, sizeof(env_res));
    env_res = generateEnv(env_res, ip, port, user, pass, way);
    char *http_prefix = "echo \"export http_proxy=\"", *https_prefix = "echo \"export https_proxy=\"";
    char *com_suffix = "\"\" >> ~/.bashrc";
    char *export1 = (char *)malloc((strlen(http_prefix) + len + sizeof(com_suffix) + 10) * sizeof(char));
    char *export2 = (char *)malloc((strlen(https_prefix) + len + sizeof(com_suffix) + 10) * sizeof(char));
    memset(export1, NULL, sizeof(export1));
    memset(export2, NULL, sizeof(export2));
    strcat(export1, http_prefix);
    strcat(export2, https_prefix);
    strcat(export1, env_res);
    strcat(export2, env_res);
    strcat(export1, com_suffix);
    strcat(export2, com_suffix);
    // printf("export1 : %s\n", export1);
    // printf("export2 : %s\n", export2);
    unsetSysProxy();
    system(export1);
    system(export2);
    free(env_res);
    free(export1);
    free(export2);
}

void unsetGitProxy() {
    system("git config --global --unset http.proxy");
    system("git config --global --unset https.proxy");
    newtWinMessage("Info", "OK", "Git proxy has been unset.");
}

void setGitProxy(const char *ip, const char *port, const char *user, const char *pass, const int way) {
    int len = strlen(ip) + strlen(port) + strlen(user) + strlen(pass) + 32;
    char *env_res = (char *)malloc(len * sizeof(char));
    memset(env_res, NULL, sizeof(env_res));
    env_res = generateEnv(env_res, ip, port, user, pass, way);
    char *http_prefix = "git config --global http.proxy '", *https_prefix = "git config --global http.proxy '";
    char *com_suffix = "'";
    char *export1 = (char *)malloc((strlen(http_prefix) + len + sizeof(com_suffix) + 10) * sizeof(char));
    char *export2 = (char *)malloc((strlen(https_prefix) + len + sizeof(com_suffix) + 10) * sizeof(char));
    memset(export1, NULL, sizeof(export1));
    memset(export2, NULL, sizeof(export2));
    strcat(export1, http_prefix);
    strcat(export2, https_prefix);
    strcat(export1, env_res);
    strcat(export2, env_res);
    strcat(export1, com_suffix);
    strcat(export2, com_suffix);
    // printf("export1 : %s\n", export1);
    // printf("export2 : %s\n", export2);
    unsetSysProxy();
    system(export1);
    system(export2);
    free(env_res);
    free(export1);
    free(export2);
}

void proxyToolsSys() {
    newtComponent main_form, protocol_form;
    newtComponent sys_info_entry, proxy_ip_entry, proxy_port_entry;
    newtComponent sys_info_label, proxy_ip_label, proxy_port_label;
    newtComponent label_1, label_2, protocol_label, sys_user_label, sys_pass_label;
    newtComponent sys_user_entry, sys_pass_entry;
    newtComponent p1, p2, p3;
    newtComponent unset_sys_button, set_sys_button, back_button;
    char *sys_info, *git_info, *dev_null, *ip_address_val, *port_val;
    char *sys_user_var, *sys_pass_var;
    struct newtExitStruct es;

    // get the system proxy settings
    sys_info = readSysProxyInfo();

    newtCls();
    newtDrawRootText(0, 0, "Raspberry Pi Development Toolbox");
    newtDrawRootText(0, 1, "Network - Proxy Settings");
    newtCenteredWindow(128, 16, "Proxy Configuration Tools");
    main_form = newtForm(NULL, NULL, 0);

    //^ System Proxy Settings Panel ----------------------------------------------------------------
    label_1 = newtLabel(10, 1, "[System Proxy Settings]");
    newtLabelSetColors(label_1, 4);
    newtFormAddComponents(main_form, label_1, NULL);

    sys_info_label = newtLabel(10, 3, "Current system proxy:");
    sys_info_entry = newtEntry(33, 3, sys_info, 48, (const char **)&dev_null, NEWT_FLAG_SCROLL);
    unset_sys_button = newtCompactButton(82, 3, "Unset System Proxy");
    newtFormAddComponents(main_form, sys_info_label, sys_info_entry, unset_sys_button, NULL);

    proxy_ip_label = newtLabel(10, 5, "IP Address:");
    proxy_ip_entry = newtEntry(23, 5, "", 32, &ip_address_val, NEWT_FLAG_SCROLL);
    proxy_port_label = newtLabel(57, 5, "Port:");
    proxy_port_entry = newtEntry(63, 5, "", 6, &port_val, NEWT_FLAG_SCROLL);
    newtFormAddComponents(main_form, proxy_ip_label, proxy_ip_entry, proxy_port_label, proxy_port_entry, NULL);

    sys_user_label = newtLabel(10, 7, "User:");
    sys_user_entry = newtEntry(23, 7, "", 32, &sys_user_var, NEWT_FLAG_SCROLL);
    sys_pass_label = newtLabel(57, 7, "Pass:");
    sys_pass_entry = newtEntry(63, 7, "", 16, &sys_pass_var, NEWT_FLAG_PASSWORD);
    newtFormAddComponents(main_form, sys_user_label, sys_user_entry, sys_pass_label, sys_pass_entry, NULL);
    
    protocol_label = newtLabel(10, 9, "Protocol:");
    p1 = newtRadiobutton(23, 9, "HTTP", 1, NULL);
    p2 = newtRadiobutton(33, 9, "SOCKET4", 0, p1);
    p3 = newtRadiobutton(46, 9, "SOCKET5", 0, p2);
    protocol_form = newtForm(NULL, NULL, 0);
    newtFormAddComponents(protocol_form, p1, p2, p3, NULL);
    newtFormSetBackground(protocol_form, NEWT_COLORSET_CHECKBOX);
    newtFormAddComponents(main_form, protocol_label, protocol_form, NULL);

    set_sys_button = newtButton(70, 11, "Set System Proxy");  //! Set sys button
    back_button = newtButton(94, 11, "Back");
    newtFormAddComponents(main_form, set_sys_button, back_button, NULL);
    //*---------------------------------------------------------------------------------------------
    
    do{
        newtFormRun(main_form, &es);

        if(es.reason == NEWT_EXIT_COMPONENT && es.u.co == unset_sys_button) {
            unsetSysProxy();
        } 
        else if (es.reason == NEWT_EXIT_COMPONENT && es.u.co == set_sys_button) {
            int choice = 1;
            // char tmp[100] = "";
            if (newtRadioGetCurrent(p1) == p1) {
                choice = 1;
            }
            else if(newtRadioGetCurrent(p1) ==  p2) {
                choice = 2;
            }
            else if(newtRadioGetCurrent(p1) == p3) {
                choice = 3;
            }
            // sprintf(tmp, "%d", choice);
            // newtDrawRootText(0, 4, tmp);
            setSysProxy(ip_address_val, port_val, sys_user_var, sys_pass_var, choice);
        }
    } while (es.reason != NEWT_EXIT_COMPONENT || es.u.co == unset_sys_button || es.u.co == set_sys_button);
    // newtFormDestroy(main_form);
    // newtFormDestroy(protocol_form);
}

void proxyToolsGit() {
    newtComponent main_form, protocol_form;
    newtComponent git_info_entry, proxy_ip_entry, proxy_port_entry;
    newtComponent git_info_label, proxy_ip_label, proxy_port_label;
    newtComponent label_1, label_2, protocol_label, git_user_label, git_pass_label;
    newtComponent git_user_entry, git_pass_entry;
    newtComponent p1, p2, p3;
    newtComponent unset_git_button, set_git_button, back_button;
    char *git_info, *dev_null, *ip_address_val, *port_val;
    char *git_user_var, *git_pass_var;
    struct newtExitStruct es;

    char*   szCmd       = "git config --global --get http.proxy";
    FILE*   pResultStr  = NULL;
    char    sys_info[1024] = {0};
    pResultStr = popen(szCmd, "r");
    if (NULL == pResultStr)
    {   
        printf("popen faild.");
        return -1; 
    }   
    fread(sys_info, 1, sizeof(sys_info), pResultStr);
    pclose(pResultStr);
    

    newtCls();
    newtDrawRootText(0, 0, "Raspberry Pi Development Toolbox");
    newtDrawRootText(0, 1, "Network - Proxy Settings");
    newtCenteredWindow(128, 16, "Proxy Configuration Tools");
    main_form = newtForm(NULL, NULL, 0);

    //^ Git Proxy Settings Panel ------------------------------------------------------------------
    label_1 = newtLabel(10, 1, "[Git Proxy Settings]");
    newtLabelSetColors(label_1, 4);
    newtFormAddComponents(main_form, label_1, NULL);

    git_info_label = newtLabel(10, 3, "Current Git proxy:");
    git_info_entry = newtEntry(33, 3, sys_info, 48, (const char **)&dev_null, NEWT_FLAG_SCROLL);
    unset_git_button = newtCompactButton(82, 3, "Unset Git Proxy");
    newtFormAddComponents(main_form, git_info_label, git_info_entry, unset_git_button, NULL);

    proxy_ip_label = newtLabel(10, 5, "IP Address:");
    proxy_ip_entry = newtEntry(23, 5, "", 32, &ip_address_val, NEWT_FLAG_SCROLL);
    proxy_port_label = newtLabel(57, 5, "Port:");
    proxy_port_entry = newtEntry(63, 5, "", 6, &port_val, NEWT_FLAG_SCROLL);
    newtFormAddComponents(main_form, proxy_ip_label, proxy_ip_entry, proxy_port_label, proxy_port_entry, NULL);

    git_user_label = newtLabel(10, 7, "User:");
    git_user_entry = newtEntry(23, 7, "", 32, &git_user_var, NEWT_FLAG_SCROLL);
    git_pass_label = newtLabel(57, 7, "Pass");
    git_pass_entry = newtEntry(63, 7, "", 16, &git_pass_var, NEWT_FLAG_PASSWORD);
    newtFormAddComponents(main_form, git_user_label, git_user_entry, git_pass_label, git_pass_entry, NULL);
    
    protocol_label = newtLabel(10, 9, "Protocol:");
    p1 = newtRadiobutton(23, 9, "HTTP", 1, NULL);
    p2 = newtRadiobutton(33, 9, "SOCKET4", 0, p1);
    p3 = newtRadiobutton(46, 9, "SOCKET5", 0, p2);
    protocol_form = newtForm(NULL, NULL, 0);
    newtFormAddComponents(protocol_form, p1, p2, p3, NULL);
    newtFormSetBackground(protocol_form, NEWT_COLORSET_CHECKBOX);
    newtFormAddComponents(main_form, protocol_label, protocol_form, NULL);

    set_git_button = newtButton(70, 11, "Set Git Proxy");  //! Set sys button
    back_button = newtButton(94, 11, "Back");
    newtFormAddComponents(main_form, set_git_button, back_button, NULL);
    //*---------------------------------------------------------------------------------------------
    
    do{
        newtFormRun(main_form, &es);

        if(es.reason == NEWT_EXIT_COMPONENT && es.u.co == unset_git_button) {
            unsetGitProxy();
        } 
        else if (es.reason == NEWT_EXIT_COMPONENT && es.u.co == set_git_button) {
            int choice = 1;
            // char tmp[100] = "";
            if (newtRadioGetCurrent(p1) == p1) {
                choice = 1;
            }
            else if(newtRadioGetCurrent(p1) ==  p2) {
                choice = 2;
            }
            else if(newtRadioGetCurrent(p1) == p3) {
                choice = 3;
            }
            // sprintf(tmp, "%d", choice);
            // newtDrawRootText(0, 4, tmp);
            setGitProxy(ip_address_val, port_val, git_user_var, git_pass_var, choice);
        }
    } while (es.reason != NEWT_EXIT_COMPONENT || es.u.co == unset_git_button || es.u.co == set_git_button);
    // newtFormDestroy(main_form);
    // newtFormDestroy(protocol_form);
}

