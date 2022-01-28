from os import walk, system

lampi = './bin/lampi'
examples = './examples'

filenames = next(walk(examples), (None, None, []))[2]
for n in filenames:
    system('{} {}/{}'.format(lampi,examples,n))
    print("\nend of test: {}.\n\n\n".format(n))
