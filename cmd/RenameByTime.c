
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <windows.h>

int main(int argc, char** argv)
{
  int    iRet = 0, iLen = 0;
  time_t curTime = 0;
  char*  strOld = NULL;
  char*  strExtName = NULL;
  char*  posOld = NULL;
  char*  posNew = NULL;
  char   strNew[128];
  char   strTime[32];
  char*  strVersion = NULL;
  
  if(argc < 3)
  {
    printf("usage: %s <filename> <R|D>\n", argv[0]);
    return -1;
  }
  
  strOld = argv[1];
  strVersion = argv[2];
  
  //STEP1:format the current time of the system
  memset(strTime, 0, sizeof(strTime));
  curTime = time(NULL);
  strftime(strTime, sizeof(strTime), "%Y%m%d%H%M", localtime((const time_t*)&(curTime)));
  
  //STEP2:get ext-name;
  iLen = strlen(strOld);
  posOld = strOld + iLen;
  while((posOld != strOld) && ('.' != *posOld))
  {
    posOld--;
  }
  
  if(posOld == strOld)
  {
    printf("the format of the file name is error;it must be \"fileName.extName;\"ex.:%s.exe\n", strOld);
    return -2;
  }
  
  strExtName = posOld; //with dot '.', then, *posOld = '.';
  
  //copy file name;
  memset(strNew, 0, sizeof(strNew));
  for(posOld = strOld, posNew = strNew; posOld != strExtName; posOld++, posNew++)
  {
    *posNew = *posOld;
  }
  
  //format the new file name;
  iLen = _snprintf(posNew, sizeof(strNew) - (posNew - strNew), "%s_%s.%s", strTime, strVersion, strExtName + 1);
  
  /*
  iRet = rename(strOld, strNew);
  if(iRet)
  {
    printf("error: %s\n", strerror(errno));
    return -3;
  }
  */
  iRet = CopyFile((LPCTSTR)strOld, (LPCTSTR)strNew, FALSE);
  if(FALSE == iRet)
  {
    printf("error: %u\n", GetLastError());
    return -3;
  }
  return 0;
}
