i=0

> cpuid_test_output.txt

while [ $i -lt 70 ]
do
  echo $i >> cpuid_test_output.txt
  cpuid -l 0x4FFFFFFD -s ${num} >> cpuid_test_output.txt
  echo $i
done
