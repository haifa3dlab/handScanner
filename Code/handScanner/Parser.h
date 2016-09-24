#ifndef Parser_h
#define Parser_h


/*
 * The Parser class recieve a seriel input parse it and call the desired 
 * Scanner command with the right parametrs
 * Example:     "baseTurn 60"  ==>   scanner.baseTurn(60)
 *              "doFullScan"   ==>   scanner.doFullScan()
 */
class Parser{
  public:
  Parser(){;}
  

  private:
  String readStringInput();

  
};



#endif
