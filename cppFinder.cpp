#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <vector>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <conio.h>
#include <sstream>

using namespace std;

bool isNumber(int keyCode){
    return keyCode >= 48 && keyCode <= 57;
}

bool isChar(int keyCode)
{
    return (keyCode >= 32 && keyCode <= 126) && keyCode != 124 && keyCode != 92 && keyCode != 63 && keyCode != 62 && keyCode != 60 && keyCode != 58 && keyCode != 47 && keyCode != 42;
}

string pathGetName(string path){
    string result = "";
    int length = path.size();
    bool isFirst = true;
    for (int i = length-1; i >= 0; i--)
    {
        if (path[i] != '\\')
        {
            result = path[i] + result;
            isFirst = false;
        }
        else if (!isFirst)
            break;
    }
    return result;
}

string PathModifierOneSlash(string p){
    int length = p.size();
    if (length <= 0)
        return "";
    string result = "";
    bool isSlash = false;
    for (int i = 0; i < length; i++)
    {
        if (isSlash){
            if (p[i] == '\\'){
                continue;
            }
            else{
                isSlash = false;
                result += p[i];
            }
        }
        else if (p[i] == '\\'){
            isSlash = true;
            result += p[i];
        }
        else
            result += p[i];
    }
    int rLength = result.size();
    if (result[rLength-1] != '\\')
        result += "\\";
    return result;
}

string PathModifier(string p){
    int length = p.size();
    if (length <= 0)
        return "";
    string result = "";
    bool isSlash = false;
    for (int i = 0; i < length; i++)
    {
        if (isSlash){
            if (p[i] == '\\'){
                result += p[i];
            }
            else{
                result += "\\";
                result += p[i];
            }
            isSlash = false;
        }
        else if (p[i] == '\\'){
            isSlash = true;
            result += p[i];
        }
        else
            result += p[i];
    }
    int rLength = result.size();
    if (result[rLength-1] != '\\')
        result += "\\\\";
    else if (result[rLength-2] != '\\')
        result += "\\";
    return result;
}

string pathCombine(string path,string sub){
    return PathModifier(path) + pathGetName(sub);
}

string pathBack(string path){
    string result = path;
    int length = path.size();
    bool isFirst = true;
    for (int i = length-1; i >= 0; i--)
    {
        if (path[i] == ':')
            break;
        if (isFirst && path[i] == '\\')
        {
            result.erase(i,1);
        }
        else if(path[i] != '\\'){
            isFirst = false;
            result.erase(i,1);
        }
        else
            break;
    }
    return result;
}

void Open(string path)
{
    path = "explorer.exe \"" + PathModifierOneSlash(path) + "\"";
    system(path.c_str());
}

string BackSpace(string input)
{
    if (input.size() > 0)
        return input.substr(0, input.size() - 1);
    else
        return input;
}

string Format(string inp, int length = 40)
{
    string result = "";
    if (inp.size() < length)
    {
        string space = "";
        for (int i = inp.size(); i < length; i++)
            space += " ";
        result = inp + space;
    }
    else if (inp.size() > length)
    {
        result = inp.substr(0, length - 4) + "... ";
    }
    else
    {
        result = inp;
    }
    return result;
}

string ToLower(string inp)
{
    string r = "";
    for (int i = 0; i < inp.size(); i++)
    {
        r += towlower(inp[i]);
    }
    return r;
}

vector<string> search(vector<string> inp, string key,bool caseSensitive){
    vector<string> r;
    for (int i = 0; i < inp.size(); i++)
    {
        if (caseSensitive && inp[i].find(key) != std::string::npos)
        {
            r.push_back(inp[i]);
        }else if(!caseSensitive && ToLower(inp[i]).find(ToLower(key)) != std::string::npos)
        {
            r.push_back(inp[i]);
        }
        
    }
    return r;
}

int ToInteger(string inp){
    stringstream geek(inp); 
    int x = 0; 
    geek >> x; 
    return x;
}

void clear(){
    system("cls");
}

string current_working_directory()
{
    char* cwd = _getcwd( 0, 0 ) ; // **** microsoft specific ****
    string working_directory(cwd) ;
    free(cwd) ;
    return working_directory ;
}

vector<string> GetSubFolder(string path)
{
    vector<string> r;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) 
    {
        while ((ent = readdir (dir)) != NULL) 
        {
            string name = ent->d_name;
            if(name == "." || name == "..")
                continue;
            r.push_back(name);
        }
    }
    closedir (dir);
    return r;
}

bool DirectoryExists(string path){
    DIR *dir;
    if ((dir = opendir (path.c_str())) != NULL)
    {
        closedir (dir);
        return true;
    }
    else{
        closedir (dir);
        return false;
    }
}

void printHeader(){
    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << Format("index", 7) << Format("Name") << Format("Path") << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;
}

vector<string> Find(string path, string key, bool caseSensitive)
{
    path = PathModifier(path);
    if (!DirectoryExists(path))
    {
        cout << "Path Not Find!" << endl;
        return {};
    }
    vector<string> sub = GetSubFolder(path);
    sub = search(sub,key,caseSensitive);
    printHeader();
    for (int i = 0; i < sub.size(); i++)
    {
        cout << Format(to_string(i + 1) + ") ", 7) << Format(sub[i]) << Format(PathModifierOneSlash(pathCombine(path,sub[i]))) << endl;
    }

    return sub;
}

void OnlineFind(string path,string key,bool caseSensitive)
{
    path = PathModifier(path);
    if (!DirectoryExists(path))
    {
        cout << "Path Not Find!" << endl;
        return;
    }
    bool isOpening = false;
    bool isGo = false;
    string index = "";
    string error = "";
    vector<string> found;
    while (true)
    {
        clear();
        if (!isOpening && !isGo)
            cout << PathModifierOneSlash(path) << "\nkey>" << key << endl;
        else if(isOpening)
            cout << PathModifierOneSlash(path) << "\nkey>" << key << "\t OpenIndex>" << index << endl;
        else
            cout << PathModifierOneSlash(path) << "\nkey>" << key << "\t GoIndex>" << index << endl;
        
        found = Find(path,key,caseSensitive);
        printHeader();

        if (!isOpening && !isGo)
            cout << "key>" << key << endl;
        else if(isOpening)
            cout << "key>" << key << "\t OpenIndex>" << index << endl;
        else
            cout << "key>" << key << "\t GoIndex>" << index << endl;

        // Get Command
        int readkey = getch();
        if (readkey == 27)
        {
            if (isOpening || isGo)
            {
                isOpening = false;
                isGo = false;
                index = "";
            }
            else
                break;
        }
        else if (readkey == 8)
        {
            if (!isOpening && !isGo)
                key = BackSpace(key);
            else
                index = BackSpace(index);
        }
        else if (readkey == 96)
        {
            if (!isGo && !isOpening)
            {
                isGo = true;
                isOpening = false;
            }
            else if (isGo && !isOpening)
            {
                isGo = false;
                isOpening = true;
            }
            else if (!isGo && isOpening)
            {
                isGo = true;
                isOpening = false;
            }
        }
        else if (readkey == 13 && (isOpening || isGo))
        {
            if(isGo){
                if(index == "." || index == "..")
                {
                    path = PathModifier(pathBack(path));
                }
                else
                {
                    int q = ToInteger(index) - 1;
                    if(q < found.size() && q >= 0){
                        string subpath = found[q];
                        path = pathCombine(path, subpath);
                    }else{
                        cout << "out of range" << endl;
                        getch();
                    }
                }
                isGo = false;
                key = "";
            }
            else if(isOpening){
                int q = ToInteger(index) - 1;
                if(q < found.size() && q >= 0){
                    string subpath = found[q];
                    Open(pathCombine(path, subpath));
                    isOpening = false;
                }else{
                    cout << "out of range" << endl;
                    getch();
                }
            }
            index = "";
        }
        else
        {
            if (!isOpening && !isGo)
            {
                if(isChar(readkey))
                    key += (char)readkey;
            }
            else
            {
                if (isNumber(readkey) || readkey == 46)
                    index += (char)readkey;
            }
        }
    }
}

void heleper()
{
    cout << "Find folders and files by name in the current path or specified path." << endl;
    cout << "" << endl;
    cout << "Usage: Finder path=[path] key=[key] [options]" << endl;
    cout << "Usage: Finder p=[path] k=[key] [options]" << endl;
    cout << "Usage: Finder p=[path] [options]" << endl;
    cout << "Usage: Finder [options]" << endl;
    cout << "" << endl;
    cout << "[Options}" << endl;
    cout << "-online          -o       Online search." << endl;
    cout << "-casesensitive   -cs      Sensitivity to uppercase and lowercase letters." << endl;
    cout << "-help            -h       Show command line help." << endl;
    cout << "" << endl;

}

int main(int argc, char const *argv[])
{
    string path = "";
    string key = "";
    bool isOnline = false;
    bool CaseSensitive = false;
    string command = "";
    for(int i = 0; i < argc; i++)
    {
        string itemLower = ToLower(argv[i]);
        command += " " + itemLower;
        if (itemLower.find("p=") != std::string::npos)
        {
            path = itemLower.substr(2);
        }
        else if (itemLower.find("path=") != std::string::npos)
        {
            path = itemLower.substr(5);
        }
        else if (itemLower.find("k=") != std::string::npos)
        {
            key = itemLower.substr(2);
        }
        else if (itemLower.find("key=") != std::string::npos)
        {
            key = itemLower.substr(4);
        }
    }
    
    isOnline = command.find("-o") != std::string::npos || command.find("-online") != std::string::npos;
    CaseSensitive = command.find("-cs") != std::string::npos || command.find("-casesensitive") != std::string::npos;
    if (command.find("-help") != std::string::npos || command.find("-h") != std::string::npos || command.find("?") != std::string::npos)
    {
        heleper();
    }
    else if (isOnline && path != "" && key != "")
    {
        OnlineFind(path,key,CaseSensitive);
    }
    else if (path != "" && key != "")
    {
        Find(path,key,CaseSensitive);
    }
    else if (path != "")
    {
        key = "";
        OnlineFind(path,key,CaseSensitive);
    }
    else
    {
        path = current_working_directory();
        cout << path << endl;
        key = "";
        OnlineFind(path,key,CaseSensitive);
    }
    return 0;
}
