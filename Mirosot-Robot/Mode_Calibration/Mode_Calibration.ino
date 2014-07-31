const unsigned int MAX_DATA=60;
int Mov[2][MAX_DATA]={};
int pr_state, nx_state;
int incomingByte;
int len=0;
int cont=0;
int k;
void setup() 
{
  Serial.begin(9600);
}
void loop() {
  while(Serial.available() > 0)
  {
    incomingByte = Serial.read();
    switch(pr_state)
    {
      case 0:
        for(k=0; k<20; k++)
        {
          Mov[0][k]=0;
          Mov[1][k]=0;
        }
        if(incomingByte=='S')
        {
          nx_state=1;
        }
        else
        {
          nx_state=0;
        }
        break;
      case 1:
        if(incomingByte=='F' || incomingByte=='B' || incomingByte=='L' || incomingByte=='R')
        {
          nx_state=2;
          if(incomingByte=='F')
          {
            Mov[0][len]=0;
          }
          else if(incomingByte=='B')
          {
            Mov[0][len]=1;
          }
          else if(incomingByte=='L')
          { 
            Mov[0][len]=2;
          }
          else
          {
            Mov[0][len]=3;
          }
          cont=0;
        }
        else
        {
          nx_state=0;
        }
        break;
      case 2:
        if(incomingByte>='0' && incomingByte<='9')
        {
          cont=cont*10+(incomingByte-'0');
          Mov[1][len]=cont;
          nx_state=3;
        }
        else
        {
          nx_state=0;
        }
        break;
      case 3:
        if(incomingByte>='0' && incomingByte<='9')
        {
          cont=cont*10+(incomingByte-'0');
          Mov[1][len]=cont;
          nx_state=3;
        }
        else if(incomingByte=='F' || incomingByte=='B' || incomingByte=='L' || incomingByte=='R')
        {
          cont=0;
          len=len+1;
          nx_state=2;
          if(incomingByte=='F')
          {
            Mov[0][len]=0;
          }
          else if(incomingByte=='B')
          {
            Mov[0][len]=1;
          }
          else if(incomingByte=='L')
          { 
            Mov[0][len]=2;
          }
          else
          {
            Mov[0][len]=3;
          }
        }
        else if(incomingByte==';')
        {
          nx_state=0;
          for(k=0; k<=len; k++)
          {
            Serial.print(k);
            Serial.print(": ");
            Serial.print(Mov[0][k]);
            Serial.print(",");
            Serial.println(Mov[1][k]);
          }
          len=0;
        }
        else
        {
          nx_state=0;
        }
        break;
    }
  pr_state=nx_state;    
  }
}
