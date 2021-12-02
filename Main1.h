//
// Created by nisha on 11/1/2021.
//

#ifndef CACHE_NISH_MAIN1_H
#define CACHE_NISH_MAIN1_H

class cache_params{
public:
    unsigned long int block_size;
    unsigned long int l1_size;
    unsigned long int l1_assoc;
    unsigned long int l2_size;
    unsigned long int l2_assoc;
    unsigned long int replacement_policy;
    unsigned long int inclusive;
};

class block {
public:
    unsigned long int valid_bit;
    unsigned long int dirty_bit;
    unsigned long int least_recently_policy;
    unsigned long int tag;
    unsigned long int addr;
    unsigned long int optimal;
};


class cache {
public:

    struct block **cache_array;
    unsigned long int tag_size;
    unsigned long int offset_size;

    unsigned long int sets;
    unsigned long int assoc;
    unsigned long int index_size;
    unsigned long int number_of_read_hits;
    unsigned long int number_of_read_misses;
    unsigned long int number_of_reads;
    unsigned long int number_of_writes_misses;
    unsigned long int number_of_writes;
    unsigned long int writeback_memory;
    unsigned long int writebacks;
    unsigned long int nextlevel;
    unsigned long int counter;
};



class Main1 {
    cache L1;
    cache L2;
    char *trace;
    int trace_counter=0;
    std::vector<unsigned long int> trace1;
    std::vector<unsigned long int> trace2;
    cache_params params; // look at sim_cache.h header file for the the definition of class cache_params

public:

   Main1(cache_params param, char *trace_file) {
        params = param;
        trace = trace_file;
    }

    void start () {
        FILE *FP;               // File handler
        char *trace_file;       // Variable that holds trace file name;
        char rw;                // variable holds read/write type read from input file. The array size is 2 because it holds 'r' or 'w' and '\0'. Make sure to adapt in future projects
        unsigned long int addr; // Variable holds the address read from input file
        // Initialising all the values to zero

        trace_file = trace;

        if (params.l1_size != 0) {
            L1.sets = (params.l1_size) / (params.block_size * params.l1_assoc);
            L1.assoc = params.l1_assoc;
            L1.offset_size = log2(params.block_size);
            L1.index_size = log2(L1.sets);
            L1.tag_size = 32 - L1.index_size - L1.offset_size;
            L1.number_of_reads = 0;
            L1.number_of_read_misses = 0;
            L1.writebacks = 0;
            L1.number_of_writes = 0;
            L1.number_of_writes_misses = 0;
            L1.writeback_memory = 0;
            L1.counter = 1;

// Constructor to allocate memory to avoid segmentation faults
            L1.cache_array = new struct block *[L1.sets];
            for (unsigned long int i = 0; i <L1.sets; i++) {                                                                // contructor working fine for L1
                L1.cache_array[i] = new struct block[params.l1_assoc];
            }

// Initialisation for the cache and its parameter values
            for (unsigned long int i = 0; i < L1.sets; i++) {
                for (unsigned long int j = 0; j < params.l1_assoc; j++) {
                    L1.cache_array[i][j].valid_bit = 0;
                    L1.cache_array[i][j].dirty_bit = 0;
                    L1.cache_array[i][j].tag = 0;
                    L1.cache_array[i][j].least_recently_policy = 0;
                }
            }
        }


        if (params.l2_size != 0) {
            L1.nextlevel = 1;
            L2.sets = (params.l2_size) / (params.block_size * params.l2_assoc);
            L2.assoc = params.l2_assoc;
            L2.offset_size = log2(params.block_size);
            L2.index_size = log2(L2.sets);
            L2.tag_size = 32 - L2.index_size - L2.offset_size;
            L2.number_of_reads = 0;
            L2.number_of_read_misses = 0;
            L2.writebacks = 0;
            L2.number_of_writes = 0;
            L2.number_of_writes_misses = 0;
            L2.counter = 1;
            // Constructor to allocate memory to avoid segmentation faults for L2
            L2.cache_array = new struct block *[L2.sets];
            for (unsigned long int i = 0; i <L2.sets; i++) {                                                                // contructor working fine for L1
                L2.cache_array[i] = new struct block[params.l2_assoc];
            }
            // Initialisation for the L2 cache and its parameter values
            for (unsigned long int i = 0; i < L2.sets; i++) {
                for (unsigned long int j = 0; j < params.l2_assoc; j++) {
                    L2.cache_array[i][j].valid_bit = 0;
                    L2.cache_array[i][j].dirty_bit = 0;
                    L2.cache_array[i][j].tag = 0;
                    L2.cache_array[i][j].least_recently_policy = 0;
                }
            }
        }


        if (params.replacement_policy == 2){
            FILE *FP1;
            unsigned long int addr1;
            FP1 = fopen(trace, "r");
            char str1[2];
            while (fscanf(FP1, "%s %lx", str1, &addr1) != EOF) {
                unsigned long int tag1 = GetTag(addr1, L1.index_size, L1.offset_size,L1.tag_size);
                trace1.push_back(tag1);
                if (params.l2_size !=0){
                    unsigned long int tag2 = GetTag(addr1, L2.index_size, L2.offset_size,L2.tag_size);
                    trace2.push_back(tag2);
                }
            }
        }




        /***************************************************************************************************************************************************************************************/
        //printing input arguments
        /***************************************************************************************************************************************************************************************/
        printf("  ********** Simulator configuration **********\n"
               "  L1_BLOCKSIZE:                      %lu\n"
               "  L1_SIZE:                           %lu\n"
               "  L1_ASSOC:                          %lu\n"
               "  L2_SIZE:                           %lu\n"
               "  L2_ASSOC:                          %lu\n"                                            // Working Fine & Comfortable
                , params.block_size, params.l1_size, params.l1_assoc, params.l2_size, params.l2_assoc);


        if (params.replacement_policy == 0) {
            printf("  REPLACEMENT_POLICY:                LRU\n");
        } else if (params.replacement_policy == 1) {
            printf("  REPLACEMENT_POLICY:                PLRU\n");
        } else if (params.replacement_policy == 2) {
            printf("  REPLACEMENT_POLICY:                Optimal Policy\n");
        } else {
            printf("invalid entry\n");
        }

        if (params.inclusive == 0) {
            printf("  INCLUSION PROPERTY:                non-inclusive\n");
        } else if (params.inclusive == 1) {
            printf("  INCLUSION PROPERTY:                inclusive\n");
        } else {
            printf("invalid entry\n");
        }
        printf("  trace_file:                        %s\n", trace_file);

        // Open trace_file in read mode
        FP = fopen(trace_file, "r");
        if (FP == NULL) {
            // Throw error and exit if fopen() failed
            printf("Error: Unable to open file %s\n", trace_file);
            exit(EXIT_FAILURE);
        }

        char str[2];
        while (fscanf(FP, "%s %lx", str, &addr) != EOF) {
            trace_counter++;
            rw = str[0];
            read_write(L1, rw, addr, 1);
        }


        printf("\n\t===== L1 contents =====\t\n");
        for (int i = 0; i < L1.sets; i++) {
            printf("\n Set %2lu: ", i);
            for (int j = 0; j < L1.assoc; j++) {
                if (L1.cache_array[i][j].dirty_bit == 1){
                    printf(" %lx D", L1.cache_array[i][j].tag);
                }
                else{
                    printf(" %lx  ", L1.cache_array[i][j].tag);
                }
            }
        }


        if (params.l2_size !=0){
            printf("\n\t===== L2 contents =====\t\n");
            for (int i = 0; i < L2.sets; i++) {
                printf("\n Set %4lu: ", i);
                for (int j = 0; j < L2.assoc; j++){
                    if (L2.cache_array[i][j].dirty_bit == 1){
                        printf(" %lx D", L2.cache_array[i][j].tag);
                    }
                    else{
                        printf(" %lx  ", L2.cache_array[i][j].tag);
                    }
                }
            }
        }


        printf("\n\t========Simulation Results===========\t\n");
        printf("\na. number of L1 reads       	 : \t %lu", L1.number_of_reads);
        printf("\nb. number of L1 reads misses	 : \t %lu", L1.number_of_read_misses);
        printf("\nc. number of L1 writes      	 : \t %lu", L1.number_of_writes);
        printf("\nd. number of L1 write misses	 : \t %lu", L1.number_of_writes_misses);
        float l1_miss_rate = (float) (L1.number_of_read_misses + L1.number_of_writes_misses)
                             / (float) (L1.number_of_reads + L1.number_of_writes);
        printf("\ne. L1 miss rate            	 : \t %f", l1_miss_rate);
        printf("\nf. number of L1 writebacks     : \t %lu", L1.writebacks);
        int memory_traffic = L1.number_of_read_misses + L1.number_of_writes_misses + L1.writebacks;
        if (params.l2_size == 0) {
            printf("\ng. number of L2 reads		 : \t %lu", 0);
            printf("\nh. number of L2 read misses	 : \t %lu", 0);
            printf("\ni. number of L2 writes	         : \t %lu", 0);
            printf("\nj. number of L2 write misses	 : \t %lu", 0);
            printf("\nk. L2 miss rate			 : \t %lu", 0);
            printf("\nl. number of L2 writebacks       :\t %lu", 0);
        } else {
            printf("\ng. number of L2 reads		 : \t %lu", L2.number_of_reads);
            printf("\nh. number of L2 read misses	 : \t %lu", L2.number_of_read_misses);
            printf("\ni. number of L2 writes	         : \t %lu", L2.number_of_writes);
            printf("\nj. number of L2 write misses	 : \t %lu", L2.number_of_writes_misses);
            float l2_miss_rate = (float) (L2.number_of_read_misses)
                                 / (float) (L2.number_of_reads);
            printf("\nk. L2 miss rate			 : \t %f", l2_miss_rate);
            printf("\nl. number of L2 writebacks       :\t %lu", L2.writebacks);
            memory_traffic =
                    L2.number_of_read_misses + L2.number_of_writes_misses + L2.writebacks + L1.writeback_memory;
        }
        printf("\nm. total memory traffic		 : \t %lu", memory_traffic);
    }


    int GetTag(unsigned long int addr, unsigned long int index, unsigned long int offset,
               unsigned long int tag)            // Here the tag stands for the tag.size()
    {
        unsigned long int n = addr;
        int binary[32] = {0};
        unsigned long int sum = 0;
        int q = 0;
        while (n != 0) {
            binary[q] = n %
                        2;                                                                                                // 31 is the most signifcant bit
            n = n / 2;
            q++;
        }
        for (int i = 0; i < tag; i++)
            sum += pow(2, tag - i - 1) * binary[31 - i];
        return sum;
    }

    int GetIndex(unsigned long int addr, unsigned long int index, unsigned long int offset, unsigned long int tag) {

        unsigned long int n = addr;
        int binary[32] = {0};
        unsigned long int sum = 0;            // here index and tags are basically the index.size() & tag.size()
        int q = 0;                                                                            // respectively
        while (n != 0) {
            binary[q] = n %
                        2;                                                                  // 31 is the least signifcant bit
            n = n / 2;
            q++;
        }
        for (int i = offset; i < offset + index; i++)
            sum = sum + pow(2, i - offset) * binary[i];
        return sum;
    }


    void UpdateLRUPolicy(unsigned long int index, unsigned long int column, int level)        // LRU policy on hits
    {
        if (level == 1){
            L1.cache_array[index][column].least_recently_policy = L1.counter;
            L1.counter++;
        }
        if (level == 2){
            L2.cache_array[index][column].least_recently_policy = L2.counter;
            L2.counter++;
        }
    }

    int getOptimal(cache C, unsigned long int index, int level) {
        std::vector<unsigned long int> optimal;
        if (level == 1){
            optimal = trace1;
        }else{
            optimal = trace2;
        }

        for (int i=0; i<C.assoc; i++) {
            C.cache_array[index][i].optimal = 0;
            for (int j= trace_counter-1; j < optimal.size(); j++) {
                if (C.cache_array[index][i].tag == optimal[j]){
                    C.cache_array[index][i].optimal = j;
                    break;
                }
            }
        }

        int opt = 0;
        for (int k=0; k<C.assoc; k++) {
            if (C.cache_array[index][k].optimal == 0){
                return k;
            }
            else if (C.cache_array[index][opt].optimal < C.cache_array[index][k].optimal){
                opt = k;
            }
        }
        return opt;
    }

    void UpdatePolicyOnMisses(cache C, int level, unsigned long int index,
                              unsigned long int tag, char rw, unsigned long int addr) {
        int max = 0;
        for (unsigned long int j = 0; j < C.assoc; j++) {
            if (C.cache_array[index][j].valid_bit == 0) {                                          // first check if valid bit is zero or not                 // case for zero valid bit
                C.cache_array[index][j].tag = tag;                                                    // update empty with tag and valid
                C.cache_array[index][j].valid_bit = 1;
                C.cache_array[index][j].addr = addr;
                if (rw == 'r'){
                    C.cache_array[index][j].dirty_bit = 0;
                }                                                           // set bit as zero which states that it is most recently used
                else{
                    C.cache_array[index][j].dirty_bit = 1;
                }
                if (params.replacement_policy == 0) {
                    UpdateLRUPolicy(index, j, level);
                }
                if (C.nextlevel == 1) {
                    read_write(L2, 'r', addr, 2);
                }
                goto OUT1;
            }
        }

        if (params.replacement_policy == 0){
            for (int k = 1; k < C.assoc; k++) {
                if (C.cache_array[index][k].least_recently_policy <
                    C.cache_array[index][max].least_recently_policy)                                                    // finding out the max LRU value of each blocks
                    max = k;
            }
        }
        else if (params.replacement_policy == 2){
            max = getOptimal(C, index, level);
        }

        if (C.cache_array[index][max].dirty_bit == 1) {
            if (level == 1){
                L1.writebacks++;
            }
            if (level == 2){
                L2.writebacks++;
            }
        }


        if (C.nextlevel == 1) {
            unsigned long int address2 = C.cache_array[index][max].addr;
            if (C.cache_array[index][max].dirty_bit == 1) {
                read_write(L2, 'w', address2, 2);
            }
            read_write(L2, 'r', addr, 2);
        }

        if (level == 2 && params.inclusive == 1){
            unsigned long int address2 = C.cache_array[index][max].addr;
            unsigned long int tag2 = GetTag(address2, L1.index_size, L1.offset_size,L1.tag_size);                  // converts the tag to decimal and returns us the value
            unsigned long int index2 = GetIndex(address2, L1.index_size, L1.offset_size, L1.tag_size);
            for (int i=0; i<L1.assoc; i++){
                if (L1.cache_array[index2][i].tag == tag2){
                    L1.cache_array[index2][i].valid_bit = 0;
                    if(L1.cache_array[index2][i].dirty_bit == 1){
                        L1.cache_array[index2][i].dirty_bit = 0;
                        L1.writeback_memory++;
                    }
                }
            }
        }

        C.cache_array[index][max].tag = tag;                                                                    // evict the one which is has been least recently used
        C.cache_array[index][max].valid_bit = 1;
        C.cache_array[index][max].addr = addr;

        if (params.replacement_policy == 0) {
            UpdateLRUPolicy(index, max, level);
        }

        if (rw == 'r'){
            C.cache_array[index][max].dirty_bit = 0;
        }                                                           // set bit as zero which states that it is most recently used
        else{
            C.cache_array[index][max].dirty_bit = 1;
        }
        OUT1:printf("");
    }


    void read_write(cache C, char rw, unsigned long int addr,
                    int level) {    //printf("\nRead\n");																 // read case
        // Read Count Incremented

        if (level == 1) {
            if (rw == 'r') {
                L1.number_of_reads++;
            } else {
                L1.number_of_writes++;
            }
        }
        else {
            if (rw == 'r'){
                L2.number_of_reads++;
            }
            else{
                L2.number_of_writes++;
            }
        }


        unsigned long int tag = GetTag(addr, C.index_size, C.offset_size,C.tag_size);                  // converts the tag to decimal and returns us the value
        unsigned long int index = GetIndex(addr, C.index_size, C.offset_size, C.tag_size);
        if(C.sets==1){
            index = 0;
        }
        for (unsigned long int j = 0; j < C.assoc; j++){
            if (C.cache_array[index][j].tag == tag)                                        // hit case
            {
                if (params.replacement_policy == 0) {
                    UpdateLRUPolicy(index, j, level);
                }
                if (rw == 'r') {
                    goto OUT;
                }
                else {
                    C.cache_array[index][j].dirty_bit = 1;
                    goto OUT;
                }
            }
        }

        if (level == 1){
            if (rw == 'r'){
                L1.number_of_read_misses++;
            }
            else{
                L1.number_of_writes_misses++;
            }
        }
        else {
            if (rw == 'r'){
                L2.number_of_read_misses++;
            }
            else{
                L2.number_of_writes_misses++;
            }
        }

        UpdatePolicyOnMisses(C, level, index, tag, rw, addr);

        OUT:
        printf("");
    }
};


#endif //CACHE_NISH_MAIN1_H
