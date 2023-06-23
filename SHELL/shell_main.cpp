#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <memory>
#include <cstdlib> 

using namespace std;

// Function Declarations
int shell_cd(vector<string>& commandArgs);            // Change directory
int shell_help(vector<string>& commandArgs);          // Display help information
int shell_exit(vector<string>& commandArgs);          // Exit the shell
int display_history(vector<string>& commandArgs);     // Display command history
int shell_mkdir(vector<string>& commandArgs);         // Create a directory
int shell_touch(vector<string>& commandArgs);         // Create a file
int shell_edit(vector<string>& commandArgs);          // Edit a file
int shell_bgcolor(vector<string>& commandArgs);      // Change background color
int shell_fgcolor(vector<string>& commandArgs);      // Change font color
int shell_lsall(vector<string>& commandArgs);        // List all directories and files

// Built-in command names
vector<string> builtin_str = {
    "cd",
    "help",
    "exit",
    "history",
    "mkdir",
    "mkfile",
    "edit",
    "bgcolor",    
    "fgcolor",
    "lsall"     
};


// Array of built-in command functions
int (*builtin_func[])(vector<string>&) = {
    &shell_cd,
    &shell_help,
    &shell_exit,
    &display_history,
    &shell_mkdir,
    &shell_touch,
    &shell_edit,
    &shell_bgcolor,    
    &shell_fgcolor,
    &shell_lsall        
};


// Linked list node for command history
struct Node
{
  string str;
  shared_ptr<Node> next;
};
shared_ptr<Node> head = nullptr;
shared_ptr<Node> cur = nullptr;

// Function to concatenate two strings
string strAppend(const string& str1, const string& str2)
{
  return str1 + str2;
}

// Function to display command history
int display_history(vector<string>& commandArgs)
{
  shared_ptr<Node> ptr = head;
  int i = 1;
  while (ptr != nullptr)
  {
    cout << " " << i++ << " " << ptr->str << endl;
    ptr = ptr->next;
  }
  return 1;
}

// Function to get the number of built-in commands
int shell_num_builtins()
{
  return builtin_str.size();
}

// Built-in command: cd
int shell_cd(vector<string>& commandArgs)
{
  if (commandArgs.size() < 2)
  {
    // If no directory is specified, change to the root directory
    if (chdir("/") != 0)
    {
      perror("shell");
    }
  }
  else
  {
    // Change to the specified directory
    if (chdir(commandArgs[1].c_str()) != 0)
    {
      perror("shell");
    }
  }
  return 1;
}

// Built-in command: mkdir
int shell_mkdir(vector<string>& commandArgs)
{
  if (commandArgs.size() < 2)
  {
    cerr << "shell: expected argument to \"mkdir\"" << endl;
  }
  else
  {
    // Create a directory with the specified name
    if (mkdir(commandArgs[1].c_str(), 0777) != 0)
    {
      perror("shell");
    }
  }
  return 1;
}

// Built-in command: touch
int shell_touch(vector<string>& commandArgs)
{
  if (commandArgs.size() < 2)
  {
    cerr << "shell: expected argument to \"touch\"" << endl;
  }
  else
  {
    // Create a file with the specified name
    ofstream file(commandArgs[1]);
    if (!file)
    {
      perror("shell");
    }
    else
    {
      file.close();
    }
  }
  return 1;
}

// Built-in command: edit
int shell_edit(vector<string>& commandArgs)
{
  if (commandArgs.size() < 2)
  {
    cerr << "shell: expected argument to \"edit\"" << endl;
  }
  else
  {
    // Open the file in text editor
    string editor = "vi"; // Change to your preferred text editor
    string command = editor + " " + commandArgs[1];
    if (system(command.c_str()) == -1)
    {
      perror("shell");
    }
  }
  return 1;
}
// Built-in command: bgcolor
int shell_bgcolor(vector<string>& commandArgs)
{
  if (commandArgs.size() < 2)
  {
    cerr << "shell: expected argument to \"bgcolor\"" << endl;
  }
  else
  {
    // Change the background color
    string color = commandArgs[1];
    string command = "setterm -background " + color;
    if (system(command.c_str()) == -1)
    {
      perror("shell");
    }
  }
  return 1;
}

// Built-in command: fgcolor
int shell_fgcolor(vector<string>& commandArgs)
{
  if (commandArgs.size() < 2)
  {
    cerr << "shell: expected argument to \"fgcolor\"" << endl;
  }
  else
  {
    // Change the font color
    string color = commandArgs[1];
    string command = "setterm -foreground " + color;
    if (system(command.c_str()) == -1)
    {
      perror("shell");
    }
  }
  return 1;
}

// Built-in command: lsall
int shell_lsall(vector<string>& commandArgs)
{
  string command = "ls -R /";  // Recursively list all directories and files starting from the root directory
  if (system(command.c_str()) == -1)
  {
    perror("shell");
  }
  return 1;
}


// Built-in command: help
int shell_help(vector<string>& commandArgs)
{
  cout << "Type program names and arguments, and hit enter." << endl;
  cout << "The following are built-in commands:" << endl;

  // Display the list of built-in commands
  for (const auto& command : builtin_str)
  {
    cout << "  " << command << endl;
  }
  return 1;
}

// Built-in command: exit
int shell_exit(vector<string>& commandArgs)
{
  // Exit the shell
  return 0;
}

// Function to launch a non-built-in command
int shell_launch(vector<string>& commandArgs)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0)
  {
    // Child process
    vector<char*> c_args(commandArgs.size() + 1);
    for (size_t i = 0; i < commandArgs.size(); ++i)
    {
      c_args[i] = const_cast<char*>(commandArgs[i].c_str());
    }
    c_args[commandArgs.size()] = nullptr;

    // Execute the command
    if (execvp(c_args[0], c_args.data()) == -1)
    {
      perror("shell");
    }
    exit(EXIT_FAILURE);
  }
  else if (pid < 0)
  {
    // Error forking
    perror("shell");
  }
  else
  {
    // Parent process
    do
    {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

// Function to execute a command
int shell_execute(vector<string>& commandArgs)
{
  if (commandArgs.empty())
  {
    // An empty command was entered.
    return 1;
  }

  for (size_t i = 0; i < shell_num_builtins(); ++i)
  {
    // Check if the command is a built-in command
    if (commandArgs[0] == builtin_str[i])
    {
      return (*builtin_func[i])(commandArgs);
    }
  }

  // Launch a non-built-in command
  return shell_launch(commandArgs);
}



// Function to read a line of input
string shell_read_line()
{
  string line;
  if (!getline(cin, line))
  {
    if (cin.eof())
    {
      exit(EXIT_SUCCESS); // We received an EOF
    }
    else
    {
      perror("shell: getline\n");
      exit(EXIT_FAILURE);
    }
  }
  return line;
}

#define SHELL_TOK_BUFSIZE 64
#define SHELL_TOK_DELIM " \t\r\n\a"

// Function to split a line into tokens
vector<string> shell_split_line(const string& line)
{
  vector<string> tokens;
  size_t pos = 0;
  size_t found = 0;
  while ((found = line.find_first_of(SHELL_TOK_DELIM, pos)) != string::npos)
  {
    if (found != pos)
    {
      tokens.push_back(line.substr(pos, found - pos));
    }
    pos = found + 1;
  }
  if (pos < line.size())
  {
    tokens.push_back(line.substr(pos));
  }
  return tokens;
}

// Function to add a command to the history
void add_to_hist(vector<string>& commandArgs)
{
  shared_ptr<Node> ptr = make_shared<Node>();

  if (head == nullptr)
  {
    head = make_shared<Node>();
    head->str = "";

    string str1 = " ";

    if (commandArgs.size() > 0)
      head->str = strAppend(commandArgs[0], str1);

    if (commandArgs.size() > 1)
      head->str = strAppend(head->str, commandArgs[1]);

    head->next = nullptr;
    cur = head;
  }
  else
  {
    ptr = make_shared<Node>();
    string str1 = " ";

    if (commandArgs.size() > 0)
      ptr->str = strAppend(commandArgs[0], str1);

    if (commandArgs.size() > 1)
      ptr->str = strAppend(ptr->str, commandArgs[1]);

    cur->next = ptr;
    ptr->next = nullptr;
    cur = ptr;
  }
}

// Function to run the shell loop
void shell_loop()
{
  string line;
  vector<string> commandArgs;
  int status = 1;

  do
  {
    cout << "> ";
    line = shell_read_line();
    commandArgs = shell_split_line(line);
    add_to_hist(commandArgs);
    status = shell_execute(commandArgs);
  } while (status);
}

int main(int argc, char** argv)
{
  // Run command loop
  shell_loop();

  return EXIT_SUCCESS;
}
