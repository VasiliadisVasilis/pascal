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
    y: integer;
    test: record_2;
    papa: roto;
		d: array1;

begin
	test._size := papa[0,'a'].arr[0];
	test._size := d[0]; 

	if  1 <  3  then 
		begin
			x:= 20 * 5;
			x:= 10
		end
  ;
	

	if 2 < 3 then
		d[3] := 5
	;
	if ( 3 < 4 ) then
		d[5] := 12
	;
	
	{ papa[0,'a'].arr[0] := 1; }
 
end.
