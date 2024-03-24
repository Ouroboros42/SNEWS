import json
import os
import pathlib

# combine single run files in a way that is compatible with AnalyseTrials.py input format
# please set parameters for reading and output file data

def combine():
    # SET PARAMETERS PLEASE
    detector1 = "SK"
    detector2 = "SNOP"
    numTrialsExpected = 1000
    sweepRange = 0.2
    base_path = pathlib.Path(__file__).parent.parent.resolve()
    abs_path = base_path / f"TrialsData/{detector1}-vs-{detector2}_{numTrialsExpected}.json"

    data_path = pathlib.Path('./output/bigbatches')

    # get all files
    # change to the directory where the files are in os.listdir
    # read all files that have the detector names in them and end with .json
    file_names = [f for f in os.listdir(data_path) if (f.endswith('.json') and detector1 in f and detector2 in f)]

    assert len(file_names) == numTrialsExpected, f"Expected {numTrialsExpected} files, got {len(file_names)}"

    srcs_processed = []
    data = {}

    for i, name in enumerate(file_names):

        # obtain instance number (src)
        src = name.split('=')[1].split('_')[0]
        srcs_processed.append(src)

        # open and read file
        with open(data_path / name, 'r') as file:
            cur_data = json.load(file)

            # add extra info once
            if src == "0":
                data["detector1"] = detector1
                data["detector2"] = detector2
                data["num-Trials"] = numTrialsExpected
                data["sweep-range"] = sweepRange
                data["True-Time-Diff"] = cur_data["True-Time-Difference"]

            # add data
            data[src] = {}
            data[src]["Likelihoods"] = cur_data["Likelihood"]
            data[src]["Offsets"] = cur_data["Time-Difference"]


    for i in range(0, numTrialsExpected):
        if str(i) not in srcs_processed:
            print(f"WARNING: Trial number {i} was not found in the files")

    assert len(data) == numTrialsExpected + 5, f"Expected {numTrialsExpected + 5} keys, got {len(data)}"



    with open(abs_path, 'w') as file:
        json.dump(data, file)

    print(f"Data has been combined and saved to {abs_path}")

if __name__ == "__main__":
    combine()