import shlex
import subprocess
import sys
import time

REF = 'GRCh38_p13_chr21_ref.fasta'
VCF = 'GCF_000001405.39.gz'

FROM_LINE = int(6.0024e6 / 70)
TO_LINE = int((6e6+4400) / 70)
FROM = FROM_LINE * 70
TO = TO_LINE * 70

VCF_OUT = 'GRCh38_p13_chr21_6m_2K.vcf'
REF_OUT = 'GRCh38_p13_chr21_6m_2K_ref.fa'

VG_OUT = 'x.vg'
VG_MOD_OUT = 'x.mod.vg'
XG_OUT = 'x.xg'
GCSA_OUT = 'x.gcsa'
SIM_OUT = 'x_2000.sim'
SIM_OUT_error = 'x_1800E.sim'

SIM_LEN = 1800
SIM_COUNT = 50


def shell_exec(script, stdout = None):
    p = subprocess.Popen(shlex.split(script), stdout = stdout)
    out = p.communicate()
    if stdout == subprocess.PIPE:
        return out[0].decode('utf-8')
    return None

def gen_ref():
    with open(REF, 'r') as infile:
        with open(REF_OUT, 'w') as outfile:
            outfile.write(infile.readline())
            for i in range(FROM_LINE):
                infile.readline()
            for i in range(TO_LINE - FROM_LINE):
                outfile.write(infile.readline())

def gen_vcf():
    # extract vcf header
    with open(VCF_OUT, 'w') as outfile:
        shell_exec(f'tabix -H {VCF}', outfile)

    # extract vcf content
    out = shell_exec(f'tabix {VCF} NC_000021.9:{FROM}-{TO}', subprocess.PIPE)

    with open(VCF_OUT, 'a') as outfile:
        for line in out.split('\n'):
            items = line.split('\t')
            if len(items) != 8:
                break
            items[1] = str(int(items[1]) - FROM)
            outfile.write('\t'.join(items) + '\n')

    # bgzip vcf
    shell_exec(f'bgzip {VCF_OUT}', subprocess.DEVNULL)

    # generate index for zipped vcf
    shell_exec(f'tabix -p vcf {VCF_OUT}.gz', subprocess.DEVNULL)

def gen_vg():
    print('Generating .vg file ...')
    starttime = time.time()
    with open(VG_OUT, 'w') as outfile:
        shell_exec(f'./vg construct -r {REF_OUT} -v {VCF_OUT}.gz', outfile)
    endtime = time.time()
    print(f'Done (duration = {endtime-starttime:.2f}s)')

def gen_index():
    print('Generating .xg file ...')
    starttime = time.time()
    shell_exec(f'./vg index -x {XG_OUT} {VG_OUT}')
    endtime = time.time()
    print(f'Done (duration = {endtime-starttime:.2f}s)')

    print('Generating .gcsa file ...')
    starttime = time.time()
    with open(VG_MOD_OUT, 'w') as outfile:
        shell_exec(f'./vg mod -pl 16 -e 3 {VG_OUT}', outfile)
    shell_exec(f'./vg index -g {GCSA_OUT} -k 16 {VG_MOD_OUT} -Z 128 -t 24 -p')
    endtime = time.time()
    print(f'Done (duration = {endtime-starttime:.2f}s)')
    
def gen_sim():
    print('Generating .sim file ...')
    starttime = time.time()
    out = shell_exec(f'./vg sim -n {SIM_COUNT} -l {SIM_LEN} -x {XG_OUT}', subprocess.PIPE)
    with open(SIM_OUT, 'w') as outfile:
        count = 1
        for line in out.split('\n'):
            if count > SIM_COUNT:
                break
            outfile.write(f'>simulated.{count}\n')
            outfile.write(line + '\n')
            count += 1
    endtime = time.time()
    print(f'Done (duration = {endtime-starttime:.2f}s)')

def gen_error():
    print('Generating .sim file ...')
    starttime = time.time()
    out = shell_exec(f'./vg sim -n {SIM_COUNT} -l {SIM_LEN} -e 0.05 -i 0.02 -x {XG_OUT}', subprocess.PIPE)
    with open(SIM_OUT_error, 'w') as outfile:
        count = 1
        for line in out.split('\n'):
            if count > SIM_COUNT:
                break
            outfile.write(f'>simulated.{count}\n')
            outfile.write(line + '\n')
            count += 1
    endtime = time.time()
    print(f'Done (duration = {endtime-starttime:.2f}s)')


if __name__ == '__main__':
    if len(sys.argv) == 1:
        gen_ref()
        gen_vcf()
        gen_vg()
        gen_index()
        gen_sim()
        gen_error()
    else:
        if sys.argv[1] == 'ref':
            gen_ref()
        elif sys.argv[1] == 'vcf':
            gen_vcf()
        elif sys.argv[1] == 'vg':
            gen_vg()
            gen_index()
            gen_sim()
        elif sys.argv[1] == 'index':
            gen_index()
        elif sys.argv[1] == 'sim':
            gen_sim()
        elif sys.argv[1] == 'error':
            gen_error()    
