# -*- coding: utf-8 -*-

###### pip install parse ####
from ctypes import Array
from parse import *

PACKETNUM : int = 0

ProxyDefine : str = "OreoPizza::Proxy::"
StubDefine : str = "OreoPizza::Stub::"

PacketDefineHeaderStr : str = ""
ProxyHeaderStr : str = ""
ProxyCppStr : str = ""
StubHeaderStr: str = "\tprivate: \tvoid ProcessRecvMessage(); \n"
StubCppStr : str = ""
StubCppSwitchStr : str = ""

def WritePacketDefineHeader():
    global PacketDefineHeaderStr
    ########################################################
    # ./PacketDefine.h 파일 작성 
    PacketDefineHeader = open("./PacketDefine.h", "w", encoding="utf-16") 
    PacketDefineHeader.write("#ifndef __PACKET_DEFINE__ \n")
    PacketDefineHeader.write("#define __PACKET_DEFINE__ \n\n") 
    PacketDefineHeader.write(PacketDefineHeaderStr)
    PacketDefineHeader.write("\n#endif")
    PacketDefineHeader.close() 
    
def WriteProxyHeader():
    global ProxyHeaderStr
    
    ########################################################
    # ./Proxy.h 파일 작성 
    ProxyHeader = open("./Proxy.h", "w", encoding="utf-16")
    ProxyHeader.write("#ifndef __PROXY_H__ \n")
    ProxyHeader.write("#define __PROXY_H__ \n\n")

    ProxyHeader.write("namespace OreoPizza \n")
    ProxyHeader.write("{ \n")
    
    ProxyHeader.write("\tclass Proxy \n")
    ProxyHeader.write("\t{ \n")
    
    ProxyHeader.write(ProxyHeaderStr)
    
    ProxyHeader.write("\t}; \n")

    ProxyHeader.write("} \n")
    
    ProxyHeader.write("\n#endif")
    ProxyHeader.close()

def WriteProxyCpp():
    global ProxyCppStr

    ########################################################
    # ./Proxy.cpp 파일 작성
    ProxyCpp = open("./Proxy.cpp", "w", encoding="utf-16")
    ProxyCpp.write("#include \"proxy.h\" \n\n")
    ProxyCpp.write(ProxyCppStr)
    ProxyCpp.close()

def WriteStubHeader():
    global StubHeaderStr

    ########################################################
    # ./stup.h 파일 작성
    StubHeader = open("./Stub.h", "w", encoding="utf-16")
    StubHeader.write("#ifndef __STUB_H__ \n")
    StubHeader.write("#define __STUB_H__ \n\n")
    
    StubHeader.write("namespace OreoPizza \n")
    StubHeader.write("{ \n")
    
    StubHeader.write("\tclass Stub \n")
    StubHeader.write("\t{ \n")
    
    StubHeader.write(StubHeaderStr)

    StubHeader.write("\t}; \n")

    StubHeader.write("} \n")

    StubHeader.write("#endif \n")
    StubHeader.close()
 
def WriteStubCpp():
    global StubCppStr
    

    StubCpp = open("./Stub.cpp", "w", encoding="utf-16")
    StubCpp.write("#include \"Stub.h\" \n\n")

    WriteStubProcessRecvMessage(StubCpp)

    StubCpp.write(StubCppStr)

    StubCpp.close()    

def WriteStubProcessRecvMessage(StubCpp):
    global StubCppSwitchStr

    StubCpp.write("void OreoPizza::Stub::ProcessRecvMessage() \n{ \n")
    
    # 메시지에서 PacketType 뽑아내기
    #
    
    StubCpp.write(StubCppSwitchStr)

    StubCpp.write("\tdefault: \n")
    # 이상한 타입의 예외처리~~~~
    StubCpp.write("\t\tbreak; \n")
    StubCpp.write("\t} \n")
    StubCpp.write("} \n")

def SaveFile():


    ########################################################
    # ./PacketDefine.h 파일 작성 
    WritePacketDefineHeader()
    
    ########################################################
    # ./Proxy.h 파일 작성 
    WriteProxyHeader()

    ########################################################
    # ./Proxy.cpp 파일 작성
    WriteProxyCpp()

    ########################################################
    # ./stup.h 파일 작성
    WriteStubHeader()

    ########################################################
    # ./stup.cpp 파일 작성
    WriteStubCpp()




    


    
   
def MyKeyWord(line : str, option : str):
    global PACKETNUM
    KeyWord, value = line.split(" ")

    if KeyWord == "#PACKETNUM":
        PACKETNUM = int(value)
        return
    elif KeyWord == "#NOBUFF" and option == "ProxyHeader":
        return 


def SetProxyDefine(FuncName : str):
    global PACKETNUM
    global PacketDefineHeaderStr
    
    PacketDefineHeaderStr += "#define " + "df" + FuncName + "\t\t\t\t" + str(PACKETNUM) + " \n "



def SetProxyHeader(DataType, FuncName, Param : Array):
    global ProxyHeaderStr

    ProxyHeaderStr += "\t\tpublic: \t\t" + DataType + " " + FuncName + "("

    # 파라미터에서 키워드를 제거하여 만들어야 한다.


    ProxyHeaderStr += "); \n"


    
    

def ParsingFunc(line : str):
    global PACKETNUM

    result = parse("{DataType} {FuncName}({Param})", line)
    print(result)

    DataType = result["DataType"]
    FuncName = result["FuncName"].strip()
    Param   = result["Param"]
    

    # 1. PacketDefine.h 파일에 작성할 값을 작성한다. 
    SetProxyDefine(FuncName.upper())

    # 2. Proxy.h 파일에 작성할 값을 작성한다. 
    SetProxyHeader(DataType, FuncName, Param)
    


    PACKETNUM += 1

    

    


def main():

    File = open("./IDL.conf", 'r', encoding = 'utf-8')
    while True:
        line = File.readline()
        if not line: break 

        # 엔터를 누른 빈 줄이면 건너뛴다.
        if line == '\n':
            continue
        # '//' 로 시작하면 주석이다. 
        elif line[0:2] == "//":
            continue
        # '#' 로 시작하면 내가 등록한 나만의 키워드이다.
        elif line[0] == "#":
            MyKeyWord(line, "")
        # 엔터를 누른 빈 줄이면 건너뛴다.
        else:
            ParsingFunc(line.strip())
            
    File.close()



    

if __name__ == "__main__":
    main()
    SaveFile()
