// C4B output: |[0,n]|

int __VERIFIER_nondet_int();
unsigned int tick = 0;

void start(int n)
{
  while (n>0) {
    n=n-1;
    while (n>0) {
	  tick++;
      if (__VERIFIER_nondet_int()) break;
      n=n-1;
    }
  }
}

int main() 
{
	start(20);
	return 0;
}