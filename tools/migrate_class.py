#!/usr/bin/env python
# Migrate scratch functions (by class prefix) out of ClusterScanOps*.cpp into a target home file.
# Usage: migrate_class.py <ClassPrefix> <TargetFile> [--create] [--preamble "extern line1; extern line2;"]
import re, os, sys

SRCS = [r"C:\dev\lemball-decomp\src\Control\ClusterScanOps.cpp",
        r"C:\dev\lemball-decomp\src\Control\ClusterScanOps2.cpp"]

def extract_blocks(src_text, prefix):
    blocks=[]
    for m in re.finditer(r'// FUNCTION: LEMBALL 0x([0-9a-fA-F]{6,8})\n', src_text):
        addr=m.group(1)
        after=src_text[m.end():]
        sig=re.match(r'(\w[\w \t\*]*?)\s+__fastcall\s+([A-Za-z_]\w*)\s*\(', after)
        if not sig: continue
        ret=sig.group(1); fid=sig.group(2)
        if not fid.startswith(prefix+"_"): continue
        rest=fid[len(prefix)+1:]
        if not rest or not rest[0].isupper(): continue
        # balanced-brace from first '{' after signature
        sigend=m.end()+sig.end()
        lb=src_text.find('{', sigend)
        if lb<0: continue
        depth=0; i=lb
        while i<len(src_text):
            c=src_text[i]
            if c=='{': depth+=1
            elif c=='}':
                depth-=1
                if depth==0:
                    block_end=i+1; break
            i+=1
        # capture from anchor through block, including trailing newline(s)
        block=src_text[m.start():block_end]
        blocks.append((fid, block, addr))
    return blocks

if __name__=="__main__":
    prefix=sys.argv[1]; tgt=sys.argv[2]
    create="--create" in sys.argv
    preamble=None
    for a in sys.argv:
        if a.startswith("--preamble="): preamble=a.split("=",1)[1]
    if not os.path.isabs(tgt): tgt=os.path.join(r"C:\dev\lemball-decomp",tgt)

    # gather blocks from both scratch files
    all_blocks={}
    for SRC in SRCS:
        if not os.path.exists(SRC): continue
        t=open(SRC,encoding='utf-8').read()
        bl=extract_blocks(t,prefix)
        # remove each block from source (by exact text, first occurrence)
        for fid,block,addr in bl:
            t2=t.replace(block,"",1)
            if t2==t:
                print(f"WARN: could not remove {addr} {fid} from {os.path.basename(SRC)}"); continue
            t=t2
        open(SRC,"w",encoding='utf-8',newline='').write(t)
        for fid,block,addr in bl: all_blocks[addr]=(fid,block)
    if not all_blocks:
        print(f"no blocks for {prefix}"); sys.exit(1)

    # append to target
    body="\n".join(b for _,b in all_blocks.values())
    was_new = not os.path.exists(tgt)
    mode="w" if (create or was_new) else "a"
    with open(tgt,mode,encoding='utf-8',newline='') as f:
        if mode=="w":
            f.write("// Migrated from ClusterScanOps2.cpp\n")
            if preamble: f.write(preamble+"\n")
            f.write("\n")
            f.write(body)
            f.write("\n")
        else:
            # ensure trailing blank line before append
            cur=open(tgt,encoding='utf-8').read()
            sep="\n" if not cur.endswith("\n\n") else ""
            f.write(sep+body+"\n")
    print(f"migrated {len(all_blocks)} {prefix} functions -> {os.path.basename(tgt)} (created={was_new or create})")
