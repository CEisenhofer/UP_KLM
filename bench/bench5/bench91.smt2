(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or (not v0) (not v1)) (or v1 (not v2))))
(assert (edge (or v2 (not v3)) (not v2)))
(assert (edge true v0))
(assert (edge (or v0 v2) (or v0 (not v2))))
(assert (edge (or (not v1) (not v2)) true))
(assert (not (edge true (or v2 (not v3) (not v0)))))