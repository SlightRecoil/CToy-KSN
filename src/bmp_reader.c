#include <ctoy.h> // ctoy API (including frequently used ANSI C libs)

void ctoy_begin() // called at the beginning of the program
{
   printf("Hello, World!\n");
   const char* fname = "data/tetris.bmp"; // or tmpnam(NULL);
   int is_ok = EXIT_FAILURE;

   FILE* fp = fopen(fname, "w+");
   if (!fp)
   {
       perror("File opening failed");
       return;
   }

   // find size of file
   if(fseek(fp,0L,SEEK_END)!=0)
   {
      perror("File opening failed");
      fclose(fp);
      return;
   }
   size_t size = ftell(fp);
   printf("%u\n",size);
   rewind(fp);

   // define target
   uint8_t * ptr = malloc(size);

   // read file
   fread(ptr, sizeof uint8_t, size, fp);
   // fehlerbehandlung malloc, fread
   // in einer funktion: readFileToMemory(const char * filename)
   // returnwert: struct{ptr, size} ; typedef MEMPTR
   // uint8_t readBYTE( uint8_t *data, size_t offset)
   


   //fread(b, sizeof b[0], SIZE, fp); 

   fclose(fp);

}

void ctoy_main_loop() // called at every update of the main loop
{}

void ctoy_end() // called at the end of the program
{}
