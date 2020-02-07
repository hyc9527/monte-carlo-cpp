import os
import subprocess
import platform


compiler = 'g++' # g++ for C++, gcc for C
file_to_compile = 'example.cpp' 
libraries_to_link = [] # add any required libraries here
libraries_that_need_compiling = ['monte_carlo.cpp'] # any cpp/c libraries in your project that you need compiled and linked

#__________________________________________________

class Error(Exception):
	"""Base exception class"""
	pass

class CompileError(Error):
	"""GCC/G++ could not compile your files"""
	pass

libstring = ''
for library in libraries_to_link:
	library = ' -l' + library
	libstring += library
astring = ''
for library in libraries_that_need_compiling:
	library = ' ' + library.split('.')[0] + '.a'
	astring += library


def execute(cmd):
	p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
	p.wait()
	if p.returncode != 0:
		print("\nYou have an error somewhere...")
		raise CompileError

if __name__ == "__main__":
	pform = platform.platform().upper()
	print('Running on: ' + pform + '\n')
	if 'LINUX' not in pform and 'UNIX' not in pform:
		print('Your OS is not supported...good luck!')
		while True:
			response = input('Proceed? (Y/N)').lower()
			if response == 'n':
				print('Quitting...')
				quit()
			if response == 'y':
				break
	enable_sudo = False
	while True:
		user_input = input('Compile and run me? (ENTER)')
		if user_input == '':
			print('Here we go!', '\n________________________________________________________\n')
			try:
				if libraries_that_need_compiling:
					print('Compiling your libraries...')
					for library in libraries_that_need_compiling:
						print("Working on:", library)
						execute(compiler + ' -c -o '  + library.split('.')[0] + '.o ' + library)
						execute('ar rcs ' + library.split('.')[0] + '.a ' + library.split('.')[0] + '.o')
				print('\nCompiling main:', file_to_compile)
				execute(compiler + ' -pthread ' + file_to_compile + libstring + astring + ' -O3')  # enable G++ optimization O3
				print('Compiled project successfully! Running...\n')
				if enable_sudo:
					os.system('sudo ./a.out')
				else:
					os.system('./a.out')
			except CompileError:
				continue
			print('\n________________________________________________________\nAll done!')
		elif user_input == 'sudo run':
			print('SUDO ENABLED')
			enable_sudo = True
		elif user_input == 'disable sudo':
			print('SUDO DISABLED')
			enable_sudo = False
