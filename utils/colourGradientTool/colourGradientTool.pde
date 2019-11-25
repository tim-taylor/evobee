int hueRed = 0;
int hueBlue = 240;
int mpMin = 300;
int mpMax = 650;
int wsize = 720;

float w;
float hs;
int hue;

void settings() {
  size(wsize, wsize);
}

void setup () {
  colorMode(HSB, 360, 100, 100);
  noLoop();
  w = ((float)wsize) / 36.0;
  hs = ((float)(hueBlue-hueRed)) / 35.0;
}

void draw() {  
  for (int i=0; i<=35; i++) {
    hue = (int)((float)hueRed + (float)(35-i)*hs);
    color c = color(hue, 100, 100);
    fill(c);
    
    int x = (int)(i*w);    
    rect(x,0,w,wsize);
        
    int r = (c >> 16) & 0xFF;  // Faster way of getting red(argb)
    int g = (c >> 8) & 0xFF;   // Faster way of getting green(argb)
    int b = c & 0xFF;          // Faster way of getting blue(argb)
    
    println("{"+(mpMin+i*10)+", {"+(((float)(r))/255)+", "+(((float)(g))/255)+", "+(((float)(b))/255)+", 255}},");
  }
}
