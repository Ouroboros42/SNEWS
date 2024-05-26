$istart = 990
$istop = 999

Import-CSV targets.csv | ForEach{
    $tdir = "output/dsbatch/$($_.Det1)-$($_.Det2)/";
    if (-not (test-path $tdir)) {
        mkdir $tdir;
    }
    
    for ($i = $istart; $i -le $istop; $i++) {
        build/run/test_run.exe $_.Det1 $_.Det2 $i -o $tdir;
    }
}

