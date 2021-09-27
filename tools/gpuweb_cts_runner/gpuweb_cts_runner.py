# gpuweb_cts_runner.py
# author: amaiorano@

import os
import sys
import subprocess
import platform
from subprocess import TimeoutExpired, check_output, CalledProcessError
import multiprocessing as mp
from multiprocessing import Pool
import signal
from pathlib import Path, PurePosixPath
from timeit import default_timer as timer


def dawn_node_path():
    if platform.system() == 'Windows':
        return 'build/RelWithDebInfo/dawnnode.node'
    if platform.system() == 'Darwin':
        return 'build/Release/dawnnode.node'
    return 'build/dawnnode.node'


# Tweakables - TODO: make args
proc_pool_size = 10 #mp.cpu_count()
test_timeout_sec = 60*5
dawnnode_module = dawn_node_path()
all_tests_file = 'all_tests.txt'
# all_tests_file = 'single_test.txt'
gpuweb_cts_rel_dir = '../gpuweb-cts'


script_dir = Path(__file__).resolve().parent
dawnnode_dir = script_dir.parent.parent.resolve()
gpuweb_cts_dir = (dawnnode_dir / gpuweb_cts_rel_dir).resolve()
run_dawn_node_js = script_dir / 'run-dawn-node.js'
all_tests_path = script_dir / all_tests_file


def run_process(cmd_args, cwd=None, timeout_sec=None):
    try:
        output = check_output(args=cmd_args, timeout=timeout_sec, stderr=subprocess.STDOUT, universal_newlines=True, cwd=cwd)
        return output
    except CalledProcessError as cpe:
        return cpe.output


def start_test_process(test_name, timeout_sec):
    cmd_args = ['node', str(run_dawn_node_js), '--verbose', test_name]
    return run_process(cmd_args, gpuweb_cts_dir, timeout_sec)


def run_test(test_name, results):
    # Run process in cts dir
    test_name = test_name.strip()

    start_time = timer()
    def end_time():
        return timer() - start_time

    try:
        output = start_test_process(test_name, timeout_sec=test_timeout_sec)
    except TimeoutExpired:
        print(f'{end_time():.2f}s [TIMEOUT]: {test_name}')
        results['timeout'].append(test_name)
        return

    found_result = False
    for line in output.splitlines():
        line = line.strip()
        if line.startswith('[fail]'):
            print(f'{end_time():.2f}s {line}')
            results['fail'].append(test_name)
            found_result = True
            break
        elif line.startswith('[pass]'):
            print(f'{end_time():.2f}s {line}')
            results['pass'].append(test_name)
            found_result = True
            break
        elif line.startswith('[skip]'):
            print(f'{end_time():.2f}s {line}')
            results['skip'].append(test_name)
            found_result = True
            break
    if not found_result:
        print(f'{end_time():.2f}s [UNKNOWN]: {test_name}')
        print(output)
        results['unknown'].append(test_name)


def clear_file_contents(f):
    p = Path(f)
    if p.exists():
        p.unlink()
    p.touch()


def to_unix(p):
    return str(p).replace('\\', '/')


def compile_gpuweb_cts():
    if not Path(gpuweb_cts_dir).exists():
        raise Exception(f"gpuweb-cts directory not found at '{gpuweb_cts_dir}'")
    print('Compiling gpuweb-cts')
    npx = 'npx.cmd' if os.name == 'nt' else 'npx'
    output = run_process([npx, 'grunt', 'run:build-out-node'], gpuweb_cts_dir)
    print(output) # TODO: get return code and fail on non-zero

    cmdline_js = Path(f'{gpuweb_cts_dir}/out-node/common/runtime/cmdline.js')
    if not cmdline_js.exists():
        raise Exception(f"Compiled js file not found at '{cmdline_js}'")


def generate_run_dawnnode_js():
    source = f"""
    require('{dawnnode_dir}/{dawnnode_module}')
    require('{gpuweb_cts_dir}/out-node/common/runtime/cmdline.js');
    """
    with open(run_dawn_node_js, 'w') as f:
        f.write(to_unix(source))


def results_filename(type):
    # e.g. <script_dir>/tests_fail_windows.txt
    return script_dir / f'tests_{platform.system().lower()}_{type}.txt'


def main():
    start_time = timer()

    os.chdir(sys.path[0]) # Run in script dir

    compile_gpuweb_cts()
    generate_run_dawnnode_js()

    all_tests = []
    with open(all_tests_path, 'r') as f:
        all_tests = f.readlines()

    original_sigint_handler = signal.signal(signal.SIGINT, signal.SIG_IGN)
    pool = Pool(proc_pool_size)
    signal.signal(signal.SIGINT, original_sigint_handler)
    try:
        # Create dictionary of lists to share across processes
        m = mp.Manager()
        results = m.dict()
        results['pass'] = m.list()
        results['fail'] = m.list()
        results['skip'] = m.list()
        results['timeout'] = m.list()
        results['unknown'] = m.list()

        for (type, values) in results.items():
            clear_file_contents(results_filename(type))

        args = [(a,results) for a in all_tests]
        pool.starmap(run_test, args)
    except KeyboardInterrupt:
        print("Caught KeyboardInterrupt, terminating processes")
        pool.terminate()
    else:
        print("Terminated normally")
        pool.close()
    pool.join()

    for (type, values) in results.items():
        file = results_filename(type)
        print(f'Writing sorted results to: {file}')
        with open(file, 'w') as f:
            values.sort()
            for v in values:
                f.write(v + '\n')

    print(f'Total elapsed time: {timer() - start_time:.2f}s')
    total = sum([len(a) for a in results.values()])
    print(f'Total tests run: {total}')
    for (type, values) in results.items():
        print(f'{type:<10}: {len(values):>5}, {len(values)/total*100:>6.2f}%')

if __name__ == '__main__':
    main()
