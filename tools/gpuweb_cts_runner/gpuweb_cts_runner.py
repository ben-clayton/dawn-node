# gpuweb_cts_runner.py
# author: amaiorano@

import io
import os
import subprocess
from subprocess import TimeoutExpired, check_output, CalledProcessError
from multiprocessing import Pool, Lock
import signal

lock = Lock()

test_timeout_sec = 120

# append_to_files_set = set()
def append_to_file(filename, text):
    with lock:
        # mode = 'a' if filename in append_to_files_set else 'w'
        # append_to_files_set.add(filename)
        mode = 'a+'
        with open(filename, mode) as f:
            f.write(text)
            if not text.endswith('\n'):
                f.write('\n')


def start_test_process(test_name, timeout_sec = 0):
    test_name = test_name.replace(r'"', r'\"') # Escape quotes or shell won't be happy
    # return subprocess.Popen('node run-dawn-node.js --verbose ' + test_name, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    cmd = 'node run-dawn-node.js --verbose ' + test_name
    try:
        output = check_output(cmd, timeout=timeout_sec, stderr=subprocess.STDOUT, text=True)
        return output
    except CalledProcessError as cpe:
        return cpe.output


def run_test(test_name):
    test_name = test_name.strip()

    try:
        output = start_test_process(test_name, timeout_sec=test_timeout_sec)
    except TimeoutExpired:
        print('[TIMEOUT]: ' + test_name)
        append_to_file('tests_timeout.txt', test_name)
        return

    found_result = False
    # for line in io.TextIOWrapper(output, encoding="utf-8"):
    for line in output.splitlines():
        line = line.strip()
        if line.startswith('[fail]'):
            print(line)
            append_to_file('tests_fail.txt', test_name)
            found_result = True
            break
        elif line.startswith('[pass]'):
            print(line)
            append_to_file('tests_pass.txt', test_name)
            found_result = True
            break
        elif line.startswith('[skip]'):
            print(line)
            append_to_file('tests_skip.txt', test_name)
            found_result = True
            break
    if not found_result:
        print('[UNKNOWN]: ' + test_name)
        append_to_file('tests_unknown.txt', test_name)

def init_worker():
    signal.signal(signal.SIGINT, signal.SIG_IGN)


def delete_file(f):
    if os.path.exists(f):
        os.remove(f)

def main():
    delete_file('tests_fail.txt') 
    delete_file('tests_pass.txt')
    delete_file('tests_skip.txt')
    delete_file('tests_unknown.txt')
    delete_file('tests_timeout.txt')

    all_tests = []
    with open('all_tests.txt', 'r') as f:
    # with open('single_test.txt', 'r') as f:
        all_tests = f.readlines()

    #original_sigint_handler = signal.signal(signal.SIGINT, signal.SIG_IGN)
    pool = Pool(100, init_worker)
    #signal.signal(signal.SIGINT, original_sigint_handler)
    try:
        pool.map(run_test, all_tests)
    except KeyboardInterrupt:
        print("Caught KeyboardInterrupt, terminating workers")
        pool.terminate()
    else:
        print("Normal termination")
        pool.close()
    pool.join()

if __name__ == '__main__':
    main()
