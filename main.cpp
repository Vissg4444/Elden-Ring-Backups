#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <thread>
#include <conio.h>

using namespace std;
namespace fs = filesystem;

static void do_a_backup()
{
   // finding elden ring folder
   string path = getenv("AppData");
   path += "\\EldenRing";

   // setting name for the archive with mask YYYYy-MMm-DDd-HHh
   string archive_name = "\\backup";
   time_t t = time(nullptr);
   char mbstr[100];
   strftime(mbstr, sizeof(mbstr), "%Yy_%mm_%dd_%Hh_%Mm", localtime(&t));
   archive_name += ("_" + string(mbstr) + ".zip ");

   // finding "newest" folder in Elden Ring saves - which being played now
   fs::file_time_type newest, last_time;
   string folder_name = "";
   for ( const auto &entry : fs::directory_iterator(path) )
   {
      const auto filenameStr = entry.path().filename().string();
      last_time = fs::last_write_time(path + "\\" + filenameStr);
      if ( last_time > newest )
      {
         newest = last_time;
         folder_name = filenameStr;
      }
   }
   path += "\\" + folder_name;

   // executing 7z command
   string command = "\"7za.exe\"  a -tzip -mx=9 " + path + "\\backups" + archive_name + path + "\\ER0000.co2 " + path + "\\ER0000.co2.bak";
   system(command.c_str());
   cout << "If you want to do a force backup, press \"y\".";
}

static void wait(float minutes)
{
   do_a_backup();
   Sleep(1000 * 60 * minutes);
   wait(minutes);
}

int main()
{
   float minutes;
   cout << "Enter number of minutes you want to wait between backups: ";
   cin >> minutes;
   thread repeat(wait, minutes);
   repeat.detach();

   while ( true )
   {
      if ( _getch() == 'y' )
         do_a_backup();
   }
   system("pause");
}