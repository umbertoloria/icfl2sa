import os

import sys

if __name__=="__main__":
    
    chr_num = sys.argv[1]
    max_size = sys.argv[2]
    parallel = sys.argv[3]

    results=[]

    os.system("python3 python_string_generator.py "+str(chr_num))

    with open("./output_xlsx","w") as output_xlsx:

        for i in range(1,int(max_size)+1):
            os.system("./main "+str(i)+" "+str(parallel)+" > output")

            with open("./output") as file:
                lines = [line.rstrip() for line in file]

            results.append(str(chr_num))
            results.append(lines[7][17:])
            results.append(lines[8][37:])
            results.append(lines[9][61:])
            results.append(lines[10][19:])
            results.append(lines[11][36:])
            results.append(lines[12][32:])
            results.append(lines[13][49:])
            results.append(lines[14][41:])
            results.append(lines[17][19:])

            res=""
            for j in range(len(results)):
                if(j==1):
                    res+=str(4)+"\t"
                res+=str(results[j])+"\t"
            res+="\n"

            output_xlsx.write(res)

            results.clear()
