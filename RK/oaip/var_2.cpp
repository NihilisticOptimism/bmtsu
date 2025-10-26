#include <iostream>
#include <vector>


int parse_num(const char *str)
{
    int result = 0;
    bool negative = false;

    while (*str == ' ')
        str++;

    if (*str == '-')
    {
        negative = true;
        str++;
    }
    else if (*str == '+')
    {
        str++;
    }

    while (*str != '\0' && *str != ';' && *str != '\n')
    {
        if (*str >= '0' && *str <= '9')
        {
            int digit = *str - '0';
            result = result * 10 + digit;
        }
        else
        {
            break;
        }
        str++;
    }

    if (negative)
        result = -result;
    return result;
}


std::string extract_cmd(const std::string &line)
{
    const char *p = line.c_str();
    const char *found = nullptr;

    while (*p != '\0')
    {
        const char *start = p;
        const char *lab = "CMD:";
        while (*start && *lab && *start == *lab)
        {
            start++;
            lab++;
        }
        if (*lab == '\0')
        {
            found = start;
            break;
        }
        p++;
    }

    if (found)
    {
        std::string cmd;
        while (*found && *found != ';')
        {
            cmd.push_back(*found);
            found++;
        }
        return cmd;
    }
    else
    {
        return ""; 
    }
}


int extract_value(const std::string &line)
{
    const char *p = line.c_str();
    const char *found = nullptr;

    while (*p != '\0')
    {
        const char *start = p;
        const char *lab = "VAL:";
        while (*start && *lab && *start == *lab)
        {
            start++;
            lab++;
        }
        if (*lab == '\0')
        {
            found = start;
            break;
        }
        p++;
    }

    if (found)
    {
        if (*found == '\0' || *found == ';')
        {
            return 0;
        }
        return parse_num(found);
    }
    else
    {
        return 0;
    }
}


int main()
{
    std::vector<std::string> stuff = {
        "CMD:SET;VAL:45",
        "CMD:GET;VAL:",
        "CMD:RESET;VAL:",
        "CMD:SET;VAL:50"
    };
    std::vector<std::string> cmds;
    std::vector<int> vals;
    
    for (int i = 0; i < stuff.size(); i++)
    {
        std::string cmd = extract_cmd(stuff[i]);
        int val = extract_value(stuff[i]);
        cmds.push_back(cmd);
        vals.push_back(val);
    }
    
    int set_count = 0;
    int set_sum = 0;
    
    for (int i = 0; i < cmds.size(); i++)
    {
        if (cmds[i] == "SET")
        {
            set_count++;
            set_sum += vals[i];
        }
    }
    
    double set_av = set_count > 0 ? static_cast<double>(set_sum) / set_count : 0.0; //посмотреть потом что за чудо здесь
    
    for (int i = 0; i < cmds.size(); i++)
    {
        std::cout << "[" << i << "] " << cmds[i] << ": " << vals[i] << std::endl;
    }
    std::cout << "SET commands: " << set_count << std::endl;
    std::cout << "Average SET value: " << set_av << std::endl;
    return 0;
}