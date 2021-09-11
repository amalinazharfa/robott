#include <mega16.h>
#include <delay.h>
#include <string.h>
#include <stdlib.h>
#include <alcd.h>
#include <stdio.h>   

#define         pwmki           OCR1A
#define         pwmka           OCR1B
#define         lampu           PORTB.3
#define         tombolkanan     PINC.3
#define         tomboltengah    PINC.4
#define         tombolkiri      PINC.5
#define         buzzer          PORTC.0

void scanka(int kec);
void scanki(int kec);
void sirka(int kec);
void sirki(int kec);

int hitung,hitbelok;
char buff1[16],buff2[16],buff3[16],buff4[16],buff5[16];
int jkiri=0, jmirki=0, jdepan=0, jmirka=0, jkanan=0, hitung=0, sensor=0, sen[8],selisih=0, gap=0;
int batasatas[8], batasbawah[8],tengah[8],x,s;
//kanandekat=125, kananjauh=315;//
int usdekat=110, usjauh=340;
//int usdekat=110, usjauh=200; 

int hitung;
void us_kiri()
{
    jkiri=0;
    PORTA.1=1; delay_us(15); PORTA.1=0;
    while(PINA.0==0){}
    while(PINA.0==1)
    { jkiri++; if(jkiri>10964){break;} }                               
    jkiri=(jkiri*0.629808565690441);
} 
void us_mirki()
{
    jmirki=0;
    PORTA.3=1; delay_us(15); PORTA.3=0;
    while(PINA.2==0){}
    while(PINA.2==1)
    { jmirki++; if(jmirki>10964){break;} }                               
    jmirki=(jmirki*0.629808565690441);
}
void us_depan()
{
    jdepan=0;
    PORTA.5=1; delay_us(15); PORTA.5=0;  //sound out
    while(PINA.4==0){} //wait
    while(PINA.4==1)
    { jdepan++; if(jdepan>10964){break;} }                               
    jdepan=(jdepan*0.629808565690441);
} 
void us_mirka()
{
    jmirka=0;
    PORTA.7=1; delay_us(15); PORTA.7=0;
    while(PINA.6==0){}
    while(PINA.6==1)
    { jmirka++; if(jmirka>10964){break;} }                               
    jmirka=(jmirka*0.629808565690441);
}
void us_kanan()
{
    jkanan=0;
    PORTC.6=1; delay_us(15); PORTC.6=0;
    while(PINC.7==0){}
    while(PINC.7==1)
    { jkanan++; if(jkanan>10964){break;} }                               
    jkanan=(jkanan*0.629808565690441);
}
void us_all()
{us_kiri();us_mirki();us_depan();us_mirka();us_kanan();}

void cek_us()
{
    us_all();
    lcd_clear();
    lcd_gotoxy(0,1);  sprintf(buff1,"%d",jkiri); lcd_puts(buff1); 
    lcd_gotoxy(0,0);  sprintf(buff2,"%d",jmirki); lcd_puts(buff2);
    lcd_gotoxy(5,0);  sprintf(buff3,"%d",jdepan); lcd_puts(buff3);
    lcd_gotoxy(10,0); sprintf(buff4,"%d",jmirka); lcd_puts(buff4);
    lcd_gotoxy(10,1); sprintf(buff5,"%d",jkanan); lcd_puts(buff5);
    delay_ms(10);
}

void rem(int nilai_rem)
{        
  PORTD.4=1;
  PORTD.5=1;
  PORTD.2=0;
  PORTD.3=0;
  PORTD.6=0;
  PORTD.7=0;
  delay_ms(nilai_rem);
 } 
void maju(unsigned char ki,unsigned char ka)
{rem(2);
        pwmka=ka;
        pwmki=ki;
        
        //dir kiri
        PORTD.2=1;
        PORTD.3=0;
        
        //dir kanan
        PORTD.6=0;
        PORTD.7=1;        
}
void maju2(unsigned char ki,unsigned char ka)
{rem(2);
        pwmka=ka;
        pwmki=ki;
        
        //dir kiri
        PORTD.2=1;
        PORTD.3=0;
        
        //dir kanan
        PORTD.6=1;
        PORTD.7=0;        
}

void maju3(unsigned char ki,unsigned char ka)
{rem(2);
        pwmka=ka;
        pwmki=ki;
        
        //dir kiri
        PORTD.2=0;
        PORTD.3=1;
        
        //dir kanan
        PORTD.6=0;
        PORTD.7=1;        
}
 
void mundur(unsigned char ki,unsigned char ka)
{ rem(2);
        pwmka=ka;
        pwmki=ki;
        
        //dir kiri
        PORTD.2=0;
        PORTD.3=1;
        
        //dir kanan
        PORTD.6=1;
        PORTD.7=0;        
}
void kanan(unsigned char ki,unsigned char ka)
{rem(2);
        pwmka=ka;
        pwmki=ki;
        
        //dir kiri
        PORTD.2=0;
        PORTD.3=1;
        
        //dir kanan
        PORTD.6=0;
        PORTD.7=1;        
}
void kiri(unsigned char ki,unsigned char ka)
{rem(2);
        pwmka=ka;
        pwmki=ki;
        
        //dir kiri
        PORTD.2=1;
        PORTD.3=0;
        
        //dir kanan
        PORTD.6=1;
        PORTD.7=0;        
}      
 void parkir()
 {  lampu=0; while(1){rem(100);} }
void konversi(){
     
     selisih=usjauh-usdekat;
     gap=selisih/6;
     batasbawah[0]=usdekat;      
     for(x=1;x<8;x++){
        batasbawah[x]=batasbawah[x-1]+gap;
     } 
     for(x=0;x<8;x++){
        batasatas[x]=batasbawah[x]+10;
     }    
       
} 
void bacasensorkanan(){
     //us_all();
     us_mirka(); 
     for(x=0;x<8;x++){
        if(jmirka>=batasbawah[x]){sen[x]=1;}
        else if (jmirka<batasbawah[x]){sen[x]=0;}
     }  
     us_kanan(); us_depan();
     sensor=(sen[7]*128)+(sen[6]*64)+(sen[5]*32)+(sen[4]*16)+(sen[3]*8)+(sen[2]*4)+(sen[1]*2)+(sen[0]*1);
     lcd_gotoxy(0,0);  sprintf(buff1,"%d%d%d%d%d%d%d%d",sen[7],sen[6],sen[5],sen[4],sen[3],sen[2],sen[1],sen[0]); lcd_puts(buff1);   
    // lcd_gotoxy(0,1);  sprintf(buff1,"%d %d %d",jdepan,jmirka,jkanan); lcd_puts(buff1); 
     //lcd_gotoxy(0,1);  sprintf(buff1,"%d ",jdepan); lcd_puts(buff1); 
     lcd_gotoxy(0,1);  sprintf(buff1,"%d ",hitung); lcd_puts(buff1); 
    
}
void bacasensorkiri(){
     //us_all();
     us_mirki(); 
     for(x=0;x<8;x++){
        if(jmirki>=batasbawah[x]){sen[x]=1;}
        else if (jmirki<batasbawah[x]){sen[x]=0;}
     }  
     us_kiri(); us_depan();
     sensor=(sen[7]*128)+(sen[6]*64)+(sen[5]*32)+(sen[4]*16)+(sen[3]*8)+(sen[2]*4)+(sen[1]*2)+(sen[0]*1);
     lcd_gotoxy(0,0);  sprintf(buff1,"%d%d%d%d%d%d%d%d",sen[7],sen[6],sen[5],sen[4],sen[3],sen[2],sen[1],sen[0]); lcd_puts(buff1);   
     lcd_gotoxy(0,1);  sprintf(buff1,"%d ",hitung); lcd_puts(buff1); 
    
}
void scanka(int kec){
  switch(sensor)        
        {          //=========>mendekat dinding kanan
                case 0b00000000: maju(kec-45,kec-49);  s=1;      break; //0
                case 0b00000001: maju(kec-40,kec-50);  s=1;      break; //1
                case 0b00000011: maju(kec-15,kec-51);  s=1;      break; //2
                case 0b00000111: maju(kec,kec-51);               break; //3
                case 0b00001111: maju(kec,kec-80);  s=0;      break; //4
                case 0b00011111: maju(kec,kec-100);  s=0;      break; //5
                case 0b00111111: maju(kec,kec-110);  s=0;      break; //6 
                case 0b01111111: maju(kec,kec-120);  s=0;      break; //7
        }   
}
void scanki(int kec){
  switch(sensor)        
        {          //=========>dekat dinding kiri
                case 0b00000000: maju(kec,kec-45);  s=1;      break; //0
                case 0b00000001: maju(kec,kec-40);  s=1;      break; //1
                case 0b00000011: maju(kec,kec-15);  s=1;      break; //2
                case 0b00000111: maju(kec,kec);               break; //3
                case 0b00001111: maju(kec-10,kec);  s=0;      break; //4
                case 0b00011111: maju(kec-20,kec);  s=0;      break; //5
                case 0b00111111: maju(kec-30,kec);  s=0;      break; //6 
                case 0b01111111: maju(kec-40,kec);  s=0;      break; //7
        }   
}
 

void sirka(int kec){
          us_mirka(); 
          us_kanan(); 
          us_depan(); 
          if(jmirka>=220 && jkanan>=180){
            maju3(180,120); //serka tajam 
            while(1){ 
                us_mirka();
                us_kanan(); 
                hitbelok=0;
                if((jmirka<=250) || (++hitbelok>30)){
                    hitbelok=0;
                    break;
                }
            buzzer=1; delay_ms(5); buzzer=0; delay_ms(20);
            }
          }              
          if(jdepan>=120){   
            bacasensorkanan();
            scanka(kec);
            us_depan();
            if (jdepan<450){
              //if(hitung>550) parkir();
              //else          
              kiri(190,190); 
              while(1){ 
                us_mirka(); 
                us_depan();
                if(jmirka>45 && jdepan>200) {
                    break;
                } 
                buzzer=1; 
                delay_ms(2); 
                buzzer=0; 
                delay_ms(20);
              } 
            }
          }
}
void sirka2(int kec){
          us_mirka(); 
          us_kanan(); 
          us_depan(); 
          if(jmirka>=180 || jkanan>=120){
            maju3(200,120); //serka tajam 
            while(1){ 
                us_mirka();
                us_kanan(); 
                hitbelok=0;
                if((jmirka<=240) || (++hitbelok>50)){
                    hitbelok=0;
                    break;
                }
            buzzer=1; delay_ms(5); buzzer=0; delay_ms(20);
            }
          }              
          if(jdepan>=120){   
            bacasensorkanan();
            scanka(kec);
            us_depan();
            if (jdepan<380){
              //if(hitung>550) parkir();
              //else          
              kiri(180,180); 
              while(1){ 
                us_mirka(); 
                us_depan();
                if(jmirka>50 && jdepan>150) {
                    break;
                } 
                buzzer=1; 
                delay_ms(2); 
                buzzer=0; 
                delay_ms(20);
              } 
            }
          }
}

void sirka_akhir(int kec){
          us_mirka(); 
          us_kanan(); 
          us_depan(); 
          if(jmirka>=190 || jkanan>=110){
            maju3(200,120); //serka tajam 
            while(1){ 
                us_mirka();
                us_kanan(); 
                hitbelok=0;
                if((jmirka<=240) || (++hitbelok>40)){
                    hitbelok=0;
                    break;
                }
            buzzer=1; delay_ms(5); buzzer=0; delay_ms(20);
            }
          }              
          if(jdepan>=120){   
            bacasensorkanan();
            scanka(kec);
            us_depan();
            if (jdepan<200){
               parkir();
              //if(hitung>550) parkir();
              //else          
              //kiri(190,190); 
              while(1){ 
                us_mirka(); 
                us_depan();
                if(jmirka>50 && jdepan>150) {
                    break;
                } 
                buzzer=1; 
                delay_ms(2); 
                buzzer=0; 
                delay_ms(20);
              } 
            }
          }
}

void sirki(int kec){
            us_mirki();  // ambil data sensor kiri yang miring
            us_kiri();   // ambil data sensor kiri 
            us_depan();  // ambil data sensor depan
          
          //untuk ketika robotnya jauh dari dinding
          if(jmirki>=260 && jkiri>=180){
            maju2(80,200); //serka taja
            while(1){ 
                us_mirki();
                us_kiri();      
                hitbelok=0;
                if((jmirki<=300) || (++hitbelok>50)){
                    hitbelok=0;
                    break;
                }
            buzzer=1; delay_ms(3); buzzer=0; delay_ms(20);
            }
          }  
          
          
          if(jdepan>=40){   
            bacasensorkiri();
            scanki(kec); 
            us_mirki();
            us_depan();
            
            if (jdepan<450){
              kanan(200,200); 
              while(1){ 
                us_mirki(); 
                us_depan();
                if(jmirki>50 && jdepan>200) {
                    break;
                } 
                buzzer=1; 
                delay_ms(2); 
                buzzer=0; 
                delay_ms(20);
              }
            }
          }
}
void sirki_akhir(int kec){
            us_mirki();  // ambil data sensor kiri yang miring
            us_kiri();   // ambil data sensor kiri 
            us_depan();  // ambil data sensor depan
          
          //untuk ketika robotnya jauh dari dinding
          if(jmirki>=260 && jkiri>=180){
            maju2(80,200); //serka taja
            while(1){ 
                us_mirki();
                us_kiri();      
                hitbelok=0;
                if((jmirki<=300) || (++hitbelok>50)){
                    hitbelok=0;
                    break;
                }
            buzzer=1; delay_ms(3); buzzer=0; delay_ms(20);
            }
          }  
          
          
          if(jdepan>=40){   
            bacasensorkiri();
            scanki(kec); 
            us_mirki();
            us_depan();
            
            if (jdepan<200){
              //kanan(200,200); 
              parkir();
              while(1){ 
                us_mirki(); 
                us_depan();
                if(jmirki>50 && jdepan>200) {
                    break;
                } 
                buzzer=1; 
                delay_ms(2); 
                buzzer=0; 
                delay_ms(20);
              }
            }
          }
} 
 
void sirki2(int kec){
            us_mirki();  // ambil data sensor kiri yang miring
            us_kiri();   // ambil data sensor kiri 
            us_depan();  // ambil data sensor depan
          
          //untuk ketika robotnya jauh dari dinding
          if(jmirki>=190 || jkiri>=110){
            maju2(100,220); //serka taja
            while(1){ 
                us_mirki();
                us_kiri();      
                hitbelok=0;
                if((jmirki<=270) || (++hitbelok>40)){
                    hitbelok=0;
                    break;
                }
            buzzer=1; delay_ms(3); buzzer=0; delay_ms(20);
            }
          }  
          
          
          if(jdepan>=50){   
            bacasensorkiri();
            scanki(kec); 
            us_mirki();
            us_depan();
            
            if (jdepan<340){
              kanan(200,200); 
              while(1){ 
                us_mirki(); 
                us_depan();
                if(jmirki>110&& jdepan>200) {
                    break;
                } 
                buzzer=1; 
                delay_ms(2); 
                buzzer=0; 
                delay_ms(20);
              }
            }
          }
} 

void main(void)
{
PORTA=0b01010101;
DDRA =0b10101010;

PORTB=0x00;
DDRB =0xFF;

PORTC=0b10111000;
DDRC =0b01000111;

PORTD=0x00;
DDRD =0xFF;


//TIMER 1, BUAT MOTOR 
TCCR1A=0xA1;
TCCR1B=0x09;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x03;

//LCD
lcd_init(16);

// Global enable interrupts
#asm("sei")
//lcd_gotoxy(0,0); lcd_puts("procommit v9"); 
lampu=1;
konversi();

while (1)
      {  
      if(tombolkiri==0){buzzer=1; delay_ms(50); buzzer=0; 
        while(tombolkiri==0){delay_ms(10);} 
        lampu=0;
        while(tombolkiri==1){ 
            cek_us();
            //lcd_gotoxy(0,0);
            //lcd_puts("tes"); 
            delay_ms(100);
            
            
        }
        delay_ms(500); 
        lcd_clear(); 
        lampu=1;
      } 
      if(tomboltengah==0){ buzzer=1; delay_ms(50);lampu=0; buzzer=0;   
        while(tomboltengah==0){delay_ms(10);}
        while(tomboltengah==1){
           
            while(++hitung<30)sirki2(250);
            rem(10);
            maju(250,150);
            delay_ms(200);
            rem(10);
             while(++hitung<130)sirka2(250);
             rem(10);
             maju(190,250);
             delay_ms(200);
             rem(10);
             while(++hitung<235)sirki2(250);
             rem(10);
             maju(250,190); 
             delay_ms(200);
             rem(10);  
             while(++hitung<440)sirka(250);   
             rem(10);   
             maju(200,230);
             delay_ms(400);
             while(++hitung<520)sirki2(250);   
             rem(10);    
             maju(190,250);
             delay_ms(200);
            rem(10);
             while(++hitung<640)sirka2(250);   
             rem(10); 
             maju(200,230); 
             delay_ms(300);
             rem(10); 
             while(++hitung<900)sirki_akhir(250);      
             
             
              
             /*
             maju(250,190);
             delay_ms(300);
             rem(10);    
             while(++hitung<900)sirki_akhir(250); 
             rem(10);
              */
             
              
              parkir();
                    
          
        } 
        buzzer=1; delay_ms(50); buzzer=0;
        lcd_clear();
        delay_ms(500);
       }
       if(tombolkanan==0){lampu=0; buzzer=1; delay_ms(50); buzzer=0;   
        while(tombolkanan==0){ delay_ms(10);}
        while(tombolkanan==1){
       
           while(++hitung<40)sirka(250);
           rem(10);
           maju(150,250); 
           delay_ms(200); 
           rem(10);  
           while(++hitung<120)sirki2(250);
           rem(10);                             
       
           maju(250,150);
           delay_ms(200);
           rem(10);
           while(++hitung<250)sirka2(250);


           rem(10);
           maju(150,250);
           delay_ms(200); 
           rem(10);
       
           while(++hitung<337)sirki(250);  
           rem(10);  
           maju(250,150); 
           delay_ms(200); 
           rem(10);    
           while(++hitung<420)sirka(250); 
           rem(10);     
           maju(150,250);
           delay_ms(200);
           rem(10);
            while(++hitung<520)sirki2(250);  
           rem(10);
           maju(250,150);
           delay_ms(200);
           rem(10);
           while(++hitung<700)sirka_akhir(250);  
           rem(10); 
           
          
            
            parkir();
            
        }             
        buzzer=1; delay_ms(50); buzzer=0;
        lcd_clear();
        delay_ms(500);
      }
      
      }
}
