# gpuweb_cts_runner.py
# author: amaiorano@

import os
import sys
import subprocess
from subprocess import TimeoutExpired, check_output, CalledProcessError
import multiprocessing as mp
from multiprocessing import Pool, Lock
import signal
from pathlib import Path, PurePosixPath
from timeit import default_timer as timer

proc_pool_size = mp.cpu_count()
test_timeout_sec = 60*5

script_dir = Path(__file__).resolve().parent
gpuweb_cts_dir = script_dir.joinpath('../../../gpuweb-cts').resolve()
dawnnode_dir = script_dir.parent.parent.resolve()

run_dawn_node_js = script_dir / 'run-dawn-node.js'

all_tests_path = script_dir / 'all_tests.txt'
# all_tests_path = script_dir / 'single_test.txt'

# tests_fail_path = script_dir / 'tests_fail.txt'
# tests_pass_path = script_dir / 'tests_pass.txt'
# tests_skip_path = script_dir / 'tests_skip.txt'
# tests_unknown_path = script_dir / 'tests_unknown.txt'
# tests_timeout_path = script_dir / 'tests_timeout.txt'

lock = Lock()

# def append_to_file(filename, text):
#     with lock:
#         with open(filename, 'a+') as f:
#             f.write(text)
#             if not text.endswith('\n'):
#                 f.write('\n')


def start_test_process(test_name, timeout_sec = 0):
    test_name = test_name.replace(r'"', r'\"') # Escape quotes or shell won't be happy
    cmd = 'node {} --verbose {}'.format(run_dawn_node_js, test_name)
    try:
        output = check_output(cmd, timeout=timeout_sec, stderr=subprocess.STDOUT, text=True, cwd=gpuweb_cts_dir)
        return output
    except CalledProcessError as cpe:
        return cpe.output


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
        # append_to_file(tests_timeout_path, test_name)
        return

    found_result = False
    for line in output.splitlines():
        line = line.strip()
        if line.startswith('[fail]'):
            print(f'{end_time():.2f}s {line}')
            # append_to_file(tests_fail_path, test_name)
            results['fail'].append(test_name)
            found_result = True
            break
        elif line.startswith('[pass]'):
            print(f'{end_time():.2f}s {line}')
            # append_to_file(tests_pass_path, test_name)
            results['pass'].append(test_name)
            found_result = True
            break
        elif line.startswith('[skip]'):
            print(f'{end_time():.2f}s {line}')
            # append_to_file(tests_skip_path, test_name)
            results['skip'].append(test_name)
            found_result = True
            break
    if not found_result:
        print(f'{end_time():.2f}s [UNKNOWN]: {test_name}')
        print(output)
        # append_to_file(tests_unknown_path, test_name)
        results['unknown'].append(test_name)


def clear_file_contents(f):
    p = Path(f)
    if p.exists():
        p.unlink()
    p.touch()


def to_unix(p):
    return str(p).replace('\\', '/')


def generate_run_dawnnode_js(dawnnode_module):
    with open(run_dawn_node_js, 'w') as f:
        source = f"""
        require('{dawnnode_dir}/{dawnnode_module}')
        require('{gpuweb_cts_dir}/src/common/tools/setup-ts-in-node.js');
        require('{gpuweb_cts_dir}/src/common/runtime/cmdline.ts');
        """
        f.write(to_unix(source))


def main():
    start_time = timer()

    os.chdir(sys.path[0]) # Run in script dir
    # clear_file_contents(tests_fail_path)
    # clear_file_contents(tests_pass_path)
    # clear_file_contents(tests_skip_path)
    # clear_file_contents(tests_unknown_path)
    # clear_file_contents(tests_timeout_path)

    dawnnode_module = 'build/RelWithDebInfo/dawnnode.node'
    generate_run_dawnnode_js(dawnnode_module)

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
            file = script_dir / f'tests_{type}.txt'
            clear_file_contents(file)
        
        args = [(a,results) for a in all_tests]
        pool.starmap(run_test, args)
    except KeyboardInterrupt:
        print("Caught KeyboardInterrupt, terminating processes")
        pool.terminate()
    else:
        print("Terminated normally")
        pool.close()
    pool.join()

    # Output results, sorted into files
    for (type, values) in results.items():
        file = script_dir / f'tests_{type}.txt'
        with open(file, 'w') as f:
            values.sort()
            for v in values:
                f.write(v + '\n')

    print(f'Total elapsed time: {timer() - start_time:.2f}s')

if __name__ == '__main__':
    main()
