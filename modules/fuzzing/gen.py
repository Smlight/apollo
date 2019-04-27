
from protofuzz import protofuzz
import string
import random

message_fuzzers = protofuzz.from_file('modules/fuzzing/proto/routing_fuzzed.proto')

for i in range(1, 31):
    i_leading0 = str(i).rjust(3, '0')
    cnt = random.randint(1, 19)
    for obj in message_fuzzers['RoutingRequest{}'.format(i_leading0)].permute():
        cnt -= 1
        if cnt == 0:
            with open('modules/fuzzing/proto/routing{}.ascii'.format(i_leading0), 'w') as fo:
                fo.write(str(obj))
            break
