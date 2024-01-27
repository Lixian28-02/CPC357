import csv

A_STATE = 'A'
B_STATE = 'B'
THRESHOLD = 60
SMALL_DIF = 5
MIN_DIS_COUNT = 4
MIN_MOT_COUNT = 4

def read_csv(file_path):
    data_list_1 = []
    data_list_2 = []
    data_list_3 = []
    total_list = []

    with open(file_path, 'r') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=' ')

        next(csv_reader, None)
        for row in csv_reader:
            data_list_1.append(float(row[0]))
            data_list_2.append(float(row[1]))
            data_list_3.append(float(row[2]))
    
    total_list.append(data_list_1)
    total_list.append(data_list_2)
    total_list.append(data_list_3)
    return total_list

def count_changes(values):
    distance = values[0]
    motion_list = values[1]
    raining_list = values[2]
    car = 0
    state = A_STATE
    dis_count = 0
    mot_count = 0
    spike = 0

    for i in range(1, len(distance)):
        dif = abs(distance[i] - distance[i-1])
        motion = motion_list[i]
        if state == A_STATE:
            if dif >= THRESHOLD:
                state = B_STATE
                dis_count = dis_count + 1
                if motion == 1:
                    mot_count = mot_count + 1
            else:
                state = A_STATE
        elif state == B_STATE:
            if dif <= SMALL_DIF and motion == 1:
                state = B_STATE
                dis_count = dis_count + 1
                mot_count = mot_count + 1
            elif dif <= SMALL_DIF and motion == 0:
                dis_count = dis_count + 1
            elif dif >= THRESHOLD:
                state = A_STATE
                if dis_count >= MIN_DIS_COUNT and mot_count >= MIN_MOT_COUNT :
                    car = car + 1
                else:
                    spike = spike + 1
                dis_count = 0
                mot_count = 0
    return car, spike

file_path = 'input.csv'
csv_data = read_csv(file_path)

count, spike = count_changes(csv_data)
print("Number of Cars Detected: ", count)
print("Number of False Positive Detected: ", spike)

