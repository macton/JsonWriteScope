#include <stdio.h>
#include <stdint.h>

#include "JsonWriteScope.h"

struct Foo;

struct Foo
{
  float a;
  int   b;
  Foo*  link;
};

const int test_data_count = 8;
Foo       test_data[ test_data_count ];

void test_setup();
void test_run();
void output_foo( JsonWriteScope* parent_scope, Foo* foo, const char* foo_name );

int main( void )
{
  test_setup();
  test_run();

  return (0);
}

void test_setup()
{
  Foo* prev = NULL;
  Foo* value;
  for (int i=0;i<test_data_count;i++)
  {
    value = &test_data[i];
    if ( prev )
    {
      prev->link = value;
    }
    value->a    = i * 1.5f;
    value->b    = i;
    value->link = NULL;
    prev = value;
  }
}

void test_run()
{
  char   output_buffer[ 4096 ];
  char*  output_start  = output_buffer;
  char*  output_cursor = output_start;
  size_t output_size   = 0;

  {
    JsonWriteScope json( output_cursor, output_size );

    output_foo( &json, &test_data[0], NULL );
  }

  fwrite( output_start, output_size, 1, stdout );
}

void output_foo( JsonWriteScope* parent_scope, Foo* foo, const char* foo_name )
{
  JsonWriteScopeObject json( parent_scope, foo_name );

  json.WriteDouble( foo->a, "a" ); 
  json.WriteInt( foo->b, "b" ); 

  if ( foo->link )
  {
    output_foo( &json, foo->link, "link" );
  }
}
