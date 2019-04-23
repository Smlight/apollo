
from protofuzz import protofuzz
import string
import random

for i in range(1, 31):
    i_leading0 = str(i).rjust(3, '0')
    message_fuzzers = protofuzz.from_file('/apollo/modules/fuzzing/proto/routing{}.proto'.format(i_leading0))
    cnt = random.randint(100, 199)
    for obj in message_fuzzers['RoutingRequest{}'.format(i_leading0)].permute():
        cnt -= 1
        if cnt == 0:
            with open('/apollo/modules/fuzzing/proto/routing{}.ascii'.format(i_leading0), 'w') as fo:
                fo.write(obj)
            break
