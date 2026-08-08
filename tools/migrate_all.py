#!/usr/bin/env python
# Batch-migrate scratch functions into per-class home files, auto-carrying needed preamble
# (externs, function externs, struct views) by scanning moved function bodies.
import re, os, glob

SRC = r"C:\dev\lemball-decomp\src\Control\ClusterScanOps2.cpp"
ROOT = r"C:\dev\lemball-decomp\src"

# class -> home file (relative to src). None => auto-create <Class>.cpp
HOME = {
    'CAI': None, 'CResFONT': None, 'CEnemy': 'AI/CEnemyGrp.cpp',
    'CBullet': None, 'CRocketManager': None, 'CPlayerLemmingGroupManager': 'AI/CPlayerLemmingGroupManager.cpp',
    'CSlinkyManager': None, 'CDuplicator': None, 'CMoverManager': None, 'CBallManager': None,
    'CGameObject': 'AI/CGameObjectMove.cpp', 'CRocket': None, 'CPaintGunManager': 'AI/CPaintGunManager.cpp',
    'CDoorManager': None, 'CCollectableManager': None, 'CPlayerLemmingGroup': 'AI/CPlayerLemmingGroup.cpp',
    'CBall': None, 'CSlinky': None, 'CAICursor': None, 'CCrate': None, 'CSheepGroup': None,
    'CSheep': None, 'CTrampolineManager': 'AI/CTrampolineManager.cpp', 'CDoor': 'AI/CDoorAct.cpp',
    'CGenericGroupManager': None, 'CTower': None, 'CBalloonPost': None, 'CMover': 'AI/CMover.cpp',
    'CBaseRemap': None,
}

txt = open(SRC, encoding='utf-8').read()

# Split preamble (everything up to first // FUNCTION:) from body
preamble_end = txt.find('// FUNCTION:')
preamble = txt[:preamble_end]
func_body = txt[preamble_end:]

# Index preamble "declare names": capture full lines defining externs / structs
preamble_names = {}  # name -> full declaration text (one or more lines)
# globals + function externs (extern ... NAME ...; or NAME( for func)
for m in re.finditer(r'extern\s+[^;]+?\b(\w+)\s*(?:\([^)]*\))?\s*;', preamble):
    preamble_names.setdefault(m.group(1), []).append(m.group(0))
# struct views
for m in re.finditer(r'struct\s+(\w+)\s*\{[^}]*\};', preamble):
    preamble_names.setdefault(m.group(1), []).append(m.group(0))
for m in re.finditer(r'struct\s+(\w+)\s*;', preamble):
    preamble_names.setdefault(m.group(1), []).append(m.group(0))

def extract_blocks(text, prefix):
    out=[]
    for m in re.finditer(r'// FUNCTION: LEMBALL 0x([0-9a-fA-F]{6,8})\n(?![//])', text):
        addr=m.group(1)
        after=text[m.end():]
        sig=re.match(r'(\w[\w \t\*]*?)\s+__fastcall\s+([A-Za-z_]\w*)\s*\(', after)
        if not sig: continue
        fid=sig.group(2)
        if not fid.startswith(prefix+'_'): continue
        rest=fid[len(prefix)+1:]
        if not rest or not rest[0].isupper(): continue
        sigend=m.end()+sig.end()
        lb=text.find('{', sigend)
        if lb<0: continue
        depth=0; i=lb
        while i<len(text):
            if text[i]=='{': depth+=1
            elif text[i]=='}':
                depth-=1
                if depth==0:
                    be=i+1; break
            i+=1
        out.append((fid, text[m.start():be], addr))
    return out

def needed_preamble(blocks):
    names=set()
    idents=set()
    for _,b,_ in blocks:
        # collect identifier-like tokens
        idents.update(re.findall(r'\b([A-Za-z_][A-Za-z0-9_]*)\b', b))
    # keep preamble names referenced
    for name in preamble_names:
        if name in idents or re.search(r'\b'+re.escape(name)+r'\b', '\n'.join(b for _,b,_ in blocks)):
            names.add(name)
    # order by first appearance in preamble
    order=[]
    for name in list(dict.fromkeys(re.findall(r'extern\s+[^;]+?\b(\w+)|\bstruct\s+(\w+)[\s;{]', preamble))):
        # flatten tuples
        nm=name[0] or name[1]
        if nm in names and nm not in order: order.append(nm)
    # include any in names not captured
    for nm in names:
        if nm not in order: order.append(nm)
    lines=[]
    for nm in order:
        lines.extend(preamble_names.get(nm, []))
    return '\n'.join(lines)

report=[]
all_migrated={}
for cls, home in HOME.items():
    blocks=extract_blocks(func_body, cls)
    if not blocks:
        report.append((cls, 0, 'skip-nosuch'))
        continue
    need=needed_preamble(blocks)
    tgt = os.path.join(ROOT, home) if home else os.path.join(ROOT, 'AI', cls+'.cpp')
    os.makedirs(os.path.dirname(tgt), exist_ok=True)
    exists=os.path.exists(tgt)
    if not exists:
        # new file: write preamble + header + blocks
        with open(tgt,'w',encoding='utf-8',newline='') as f:
            if need: f.write(need+'\n\n')
            f.write('\n'.join(b for _,b,_ in blocks)+'\n')
    else:
        cur=open(tgt,encoding='utf-8').read()
        sep='\n' if not cur.endswith('\n\n') else ''
        with open(tgt,'a',encoding='utf-8',newline='') as f:
            f.write(sep+'\n'.join(b for _,b,_ in blocks)+'\n')
    report.append((cls,len(blocks), '->'+os.path.basename(tgt)+('' if exists else ' [NEW]')))
    all_migrated[cls]=blocks

# remove migrated blocks from source
for cls, blocks in all_migrated.items():
    for _,b,_ in blocks:
        func_body=func_body.replace(b,'',1)
new_src = preamble + func_body
open(SRC,'w',encoding='utf-8',newline='').write(new_src)

print("migration report:")
for cls,n,dest in report:
    print(f"  {cls:<28} {n}  {dest}")
tot=sum(n for _,n,_ in report)
print(f"TOTAL migrated: {tot} functions")
