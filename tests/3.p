program testing;
const dim1 = 10;
      dim2 = 20;

type
  array1 = array[dim1] of integer;
  record_1= record
    int1,int2:integer;
    char1,char2:char;
    arr: array1
  end;
  record_2= record
    rec1 : record_1;
    _size : integer
  end;
  roto = array[dim1,dim2] of record_1;
var x: integer;
    y: char;
    test: record_2;
    papa: roto;

begin
  x:= papa[0,'a'].arr[0];
end.