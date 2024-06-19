
#-*- coding: cp949 -*- 

PACKETNUM = 0

def MyKeyWord(line : str):
    KeyWord = line.split(" ")

    if KeyWord[0] == "PACKETNUM":
        PACKETNUM = KeyWord[1]



def main():
   IDL = open("./IDL.txt", 'r', encoding = 'UTF-8')
   while True:
        line = IDL.readline()

        # 처음이 '#' 으로 시작하면 나만의 키워드이다.
        if line[0:2] == "//":
            continue
        elif line[0] == '#':
            MyKeyWord(line)
            

        if not line: break 
        print(line)
   IDL.close()
    

if __name__ == "__main__":
    main()
