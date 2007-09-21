# $Id: numbers1.tcl,v 1.6 2003/02/05 11:28:23 marco Exp $

set number 1000
set lst {}

for {set i 0} {$i < $number} {incr i 7} {
    lappend lst $i
}
for {set i 4} {$i < $number} {incr i 7} {
    lappend lst $i
}
for {set i 6} {$i < $number} {incr i 7} {
    lappend lst $i
}
for {set i 3} {$i < $number} {incr i 7} {
    lappend lst $i
}
for {set i 1} {$i < $number} {incr i 7} {
    lappend lst $i
}
for {set i 5} {$i < $number} {incr i 7} {
    lappend lst $i
}
for {set i 2} {$i < $number} {incr i 7} {
    lappend lst $i
}

set string {}

for {set i 0} {$i < $number} {incr i 10} {
    set range [lrange $lst $i [expr {$i+9}]]

    foreach n $range {
	append string "$n, "
    }

    append string \n
}

puts stderr [llength [lsort -unique $lst]]

append string $number
puts "
#undef LITTLENUMBER
#define LITTLENUMBER $number

int array\[\] = \{
"
puts $string
puts "\};\n"

exit 0

### end of file
