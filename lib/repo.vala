namespace Seafile {

public class Repo : Object {

    // _id is for fast access from c code. id is for
    // vala to automatically generate a property. Note,
    // if a Vala property is start with _, it is not
    // translated into a GObject property.
    public char _id[37];
    public string id {
        get { return (string)_id; }
        set { Posix.memcpy(_id, value, 36); _id[36] = '\0'; }
    }

    public string _name;
    public string name {
        get { return _name; }
        set { _name = value; }
    }

    public string _desc;        // description
    public string desc {
        get { return _desc; }
        set { _desc = value; }
    }

    public string _worktree;
    public string worktree {
        get { return _worktree; }
        set { _worktree = value; }
    }

    public string _relay_id;
    public string relay_id {
        get { return _relay_id; }
        set { _relay_id = value; }
    }

    public bool encrypted { get; set; }
    public string passwd { get; set; }
    public string magic { get; set; }

    public bool worktree_changed { get; set; }
    public bool worktree_invalid { get; set; }
    public int  worktree_checktime { get; set; }
    public int  last_sync_time { get; set; }

    public bool index_corrupted { get; set; }

    public bool auto_sync { get; set; }

    public int    last_modify { get; set; }
    public int64  size { get; set; }

    public string head_branch { get; set; }

    public string head_cmmt_id { get; set; }

    public string shared_email { get; set; }

    public string share_permission { get; set; }

    public bool no_local_history { get; set; }
}


public class SyncInfo : Object {

    public string repo_id { get; set; }
    public string head_commit { get; set; }
    public bool deleted_on_relay { get; set; }
    public bool bad_local_branch { get; set; }
    public bool need_fetch { get; set; }
    public bool need_upload { get; set; }
    public bool need_merge { get; set; }
    // public int last_sync_time { get; set; }
}

public class SyncTask : Object {

    public bool is_sync_lan { get; set; }
    public bool force_upload { get; set; }
    public string dest_id { get; set; }
    public string repo_id { get; set; }
    public string state { get; set; }
    public string error { get; set; }
    public string tx_id { get; set; }
}

public class SessionInfo : Object {
    public string datadir { get; set; }
}

public class CheckoutTask : Object {

    public string repo_id { get; set; }
    public string worktree { get; set; }
    public int total_files { get; set; }
    public int finished_files { get; set; }
}

// Unified container object for shared repo info.
// Returned by list_share_repos, get_group_repos and get_inner_pub_repos, etc
public class SharedRepo : Object {

    public string share_type { get; set; } // personal, group or public

    public string repo_id { get; set; }
    public string repo_name { get; set; }
    public string repo_desc { get; set; }
    public bool encrypted { get; set; }
    public string permission { get; set; }

    public string user { get; set; } // share from or share to
    public int group_id { get; set; } // used when shared to group

    public int last_modified { get; set; }
}

public class DiffEntry : Object {

    public string status { get; set; }
    public string name { get; set; }
    public string new_name { get; set; }
}

public class DeletedEntry : Object {

    public string commit_id { get; set; }
    public string obj_id { get; set; }
    public string obj_name { get; set; }
    public string basedir { get; set; }
    public int mode { get; set; }
    public int delete_time { get; set; }
    public int64 file_size { get; set; }
}

} // namespace
